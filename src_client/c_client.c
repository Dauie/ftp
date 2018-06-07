#include "../incl/client.h"
/*
 *  ./client
 */

char	*g_cmds[][2] = {
		{ "CWD", "cd" },
		{ "HELP", "help" },
		{ "LIST", "ls" },
		{ "PASV", "passive" },
		{ "PWD", "pwd" },
		{ "QUIT", "quit" },
		{ "RETR", "get" },
		{ "STOR", "put" },
};

int 	(*g_c_funcs[])(t_session *) = { &c_cwd, &c_help, &c_list, &c_passive,
											&c_pwd, &c_quit, &c_retrieve, &c_store};

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

int 	c_help(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	write(1, session->buff, ft_strlen(session->buff));
	return (EXIT_SUCCESS);
}

ssize_t 		read_stdin(char *buff)
{
	ssize_t ret;

	ret = 0;
	if ((ret = read(0, buff, BUFFSZ)) < 0)
		return (EXIT_FAILURE);
	buff[ret - 1] = '\0';
	return (ret);
}

static int	dispatch_userin(t_session *session, char *user_input)
{
	int i;
	int j;

	i = -1;
	while (++i < CMD_CNT)
	{
		j = -1;
		while (++j < 2)
		{
			if (ft_strncmp(g_cmds[i][j], session->argv[0], ft_strlen(session->argv[0])) == 0)
			{
				if (send_msg(session->sock, 2, g_cmds[i][0],
							 &user_input[ft_strlen(session->argv[0])]) == EXIT_FAILURE)
					return (EXIT_FAILURE);
				g_c_funcs[i](session);
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
	char	user_input[BUFFSZ];

	while (session->run == TRUE)
	{
		ft_bzero(session->buff, BUFFSZ);
		ft_bzero(user_input, BUFFSZ);
		write(1, "(^-^)> ", 7);
		if (read_stdin(user_input) == EXIT_FAILURE)
			continue;
		if (!(session->argv = ft_strsplit(user_input, ' ')) || !session->argv[0])
			continue;
		dispatch_userin(session, user_input);
		clean_session(session);
	}
	printf("[-]Disconnected from server\n");
}

int create_connection(t_session *session, char *addr)
{
	create_socket(session, addr);
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


