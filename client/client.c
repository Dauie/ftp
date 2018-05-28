#include "../incl/ftp.h"
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

static int create_connection(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	if ((proto = getprotobyname("tcp")) == 0)
		return(-1);
	if (!(sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)))
	{
		printf("[-]Error establishing socket (-.-)\n");
		exit(1);
	}
	printf("[+]Client socket created\n");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		printf("[-]Error connecting to %s:%d ...(-.-)\n", addr, port);
		exit(2);
	}
	printf("[+]Successfully connected to %s:%d\n", addr, port);
	return (sock);
}

int	main(int ac, char **av)
{
	t_session session;

	init_session(&session);
	if (ac != 3)
		usage(av[0]);
	session.port = ft_atoi(av[2]);
	session.sock = create_connection(av[1], session.port);
	client_shell(&session);
	close(session.sock);
	return(0);
}


