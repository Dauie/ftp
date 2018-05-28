#include "../incl/client.h"
/*
 *  ./client
 */

static void usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	exit(-1);
}

void		print_file(int fd)
{
	int ret;
	char *line;

	ret = 0;
	while ((ret = gnl(fd, &line)) > 0)
	{
		printf("%s", line);
		free(line);
	}
}

static void	client_shell(t_session *session)
{
	char	*user_input;

	while (TRUE)
	{
		// Get command from user
		write(1, "(^-^)> ", 7);
		if (gnl(0, &user_input) < 0)
			break;
		ft_strcpy(session->buff, user_input);
		free(user_input);
		// Send command to server
		send(session->sock, session->buff, BUFFSZ, MSG_WAITALL);
		// Close client if prompted to
		if (ft_strncmp(session->buff, "quit", 4) == 0)
			break;
		recv_file(session);
		print_file(session->fd);
		ft_bzero(session->buff, BUFFSZ);
	}
	printf("[-]Disconnected from server\n");
}

static int create_connection(char *addr, t_session *session)
{
	create_socket(session);
	bind_socket(session, addr);
	if (connect(session->sock, (const struct sockaddr *)&session->sin, sizeof(session->sin)) == -1)
	{
		printf("[-]Error connecting to %s:%d ...(-.-)\n", addr, session->port);
		return (EXIT_FAILURE);
	}
	printf("[+]Successfully connected to %s:%d\n", addr, session->port);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av)
{
	t_session session;

	init_session(&session);
	if (ac != 3)
		usage(av[0]);
	session.port = ft_atoi(av[2]);
	if (create_connection(av[1], &session) == EXIT_SUCCESS)
		client_shell(&session);
	close(session.sock);
	return(0);
}


