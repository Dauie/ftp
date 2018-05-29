#include "../incl/client.h"
/*
 *  ./client
 */

static void usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	exit(-1);
}

int		c_quit(t_session *session)
{
	session->run = FALSE;
	return (EXIT_SUCCESS);
}

static int	dispatch_userin(t_session *session, char *user_input)
{
	int i;
	int j;

	i = -1;
	j = -1;
	while (++i < CMD_CNT)
	{
		while (++j < 2)
		{
			if (ft_strcmp(g_cmds[i][j], session->argv[0]) == 0)
			{
				if (send_msg(session, 2, session->argv[0],
							 &user_input[ft_strlen(session->argv[0])]) == EXIT_FAILURE)
					return (EXIT_FAILURE);
				g_client_funcs[i](session);
				return (EXIT_SUCCESS);
			}
		}
	}
	if (i == CMD_CNT)
		printf("[-]Command not implemented\n");
	return (EXIT_SUCCESS);
}

static void	client_shell(t_session *session)
{
	char	*user_input;

	while (session->run == TRUE)
	{
		ft_bzero(session->buff, BUFFSZ);
		write(1, "(^-^)> ", 7);
		if (gnl(0, &user_input) < 0)
			break;
		if (!(session->argv = ft_strsplit(user_input, ' ')) || !session->argv[0])
			continue;
		dispatch_userin(session, user_input);
		free(user_input);
		ft_tbldel(session->argv, ft_tbllen(session->argv));
	}
	printf("[-]Disconnected from server\n");
}

static int create_connection(t_session *session, char *addr)
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
	if (create_connection(&session, av[1]) == EXIT_SUCCESS)
		client_shell(&session);
	close(session.sock);
	return(0);
}


