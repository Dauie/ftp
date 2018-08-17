/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_client.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:42:41 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 15:24:39 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/client.h"

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

int		(*g_c_funcs[])(t_session *) = { &c_dircmd, &c_help, &c_list, &c_passive,
	&c_dircmd, &c_quit, &c_retrieve, &c_store};

ssize_t			read_stdin(char *buff)
{
	ssize_t		ret;

	if ((ret = read(0, buff, BUFFSZ)) < 0)
		return (FAILURE);
	buff[ret - 1] = '\0';
	return (ret);
}

static int		dispatch_userin(t_session *session, char *user_input)
{
	int			i;
	int			j;

	i = -1;
	while (++i < CMD_CNT)
	{
		j = -1;
		while (++j < 2)
		{
			if (ft_strncmp(g_cmds[i][j], session->argv[0],
					ft_strlen(g_cmds[i][j])) == 0)
			{
				if (send_msg(session->sock, 2, g_cmds[i][0],
						&user_input[ft_strlen(session->argv[0])]) == FAILURE)
					return (FAILURE);
				g_c_funcs[i](session);
				return (SUCCESS);
			}
		}
	}
	if (i == CMD_CNT)
		dprintf(STDERR_FILENO, "[-]Invalid command %s\n", user_input);
	return (SUCCESS);
}

static void		client_shell(t_session *session)
{
	char		user_input[BUFFSZ];

	while (session->run == TRUE)
	{
		ft_bzero(session->buff, BUFFSZ);
		ft_bzero(user_input, BUFFSZ);
		write(1, "(^-^)> ", 7);
		if (read_stdin(user_input) == FAILURE)
			continue;
		if (!(session->argv = ft_strsplit(user_input, ' ')) ||
				!session->argv[0])
			continue;
		dispatch_userin(session, user_input);
		clean_session(session);
	}
	printf("[-]Disconnected from server\n");
}

int				create_connection(t_session *session, char *addr)
{
	if (create_socket(session, addr) == FAILURE)
		return (FAILURE);
	if (connect(session->sock, (const struct sockaddr *)&session->sin,
				sizeof(session->sin)) == FAILURE)
	{
		dprintf(STDERR_FILENO, "[-]Error connecting to %s:%d ...(-.-)\n",
				addr, session->port);
		return (FAILURE);
	}
	printf("[+]Successfully connected to %s:%d\n", addr, session->port);
	return (SUCCESS);
}

int				main(int ac, char **av)
{
	t_session	*session;
	extern char **environ;

	if (ac != 3)
		usage(av[0]);
	if (!(session = ft_memalloc(sizeof(t_session))))
		return (FAILURE);
	g_session = session;
	init_session(session, av, environ);
	session->port = ft_atoi(av[2]);
	signal(SIGINT, handle_killcli_sig);
	if (create_connection(session, av[1]) == SUCCESS)
		client_shell(session);
	destroy_session(session);
	free(session);
	return (SUCCESS);
}
