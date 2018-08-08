/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:45:45 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 13:45:45 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

static void		usage(char *str)
{
	printf("Useage: %s <port>\n", str);
	exit(FAILURE);
}

static void		session_manager(t_session *session)
{
	while (session->run)
	{
		if (accept_connection(session) == FAILURE)
			continue;
		if ((session->pid = fork()) == -1)
		{
			close(session->cs);
			continue;
		}
		else if (session->pid > 0)
		{
			close(session->cs);
			signal(SIGCHLD, SIG_IGN);
		}
		else if (session->pid == 0)
		{
			close(session->sock);
			manage_client_session(session);
		}
	}
}

int				s_help(t_session *session)
{
	if (send_msg(session->cs, 9, "FTP Server Usage:\n",
				"CWD <dir> - change working directory.\n",
				"HELP - Lists all supported commands\n",
				"LIST <path> - List files/directories in path\n",
				"PASV - Enter passive mode\n",
				"PWD - Prints working directory\n",
				"QUIT - Closes connection and quits program.\n",
				"RETR <path> - Retrieve file at path.\n",
				"STOR <path> - Store file at path.\n") == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int				accept_connection(t_session *session)
{
	if (!(session->cs = accept(session->sock,
					(struct sockaddr*)&session->sin, &session->cslen)))
		return (FAILURE);
	printf("[+]New connection %s:%d on sd %d\n",
		inet_ntoa(session->sin.sin_addr),
		ntohs(session->sin.sin_port), session->cs);;
	return (SUCCESS);
}

int				create_endpoint(t_session *session, char *address)
{
	if (create_socket(session, address) == FAILURE)
		return (FAILURE);
	if (bind_socket(session) == FAILURE)
		return (FAILURE);
	if (options_socket(session) == FAILURE)
		return (FAILURE);
	if (listen_socket(session) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int 			main(int ac, char **av)
{
	t_session	*session;
	extern char	**environ;

	if (ac != 2)
		usage(av[0]);
	signal(SIGINT, handel_killsvr_sig);
	if (!(session = ft_memalloc(sizeof(t_session))))
		return (FAILURE);
	init_session(session);
	g_session = session;
	if (!(session->env = ft_tbldup(environ, ft_tbllen(environ))))
		return (FAILURE);
	session->port = ft_atoi(av[1]);
	if (create_endpoint(session, NULL) == FAILURE)
		return (FAILURE);
	printf("[+]Server started on port %d\n", session->port);
	session_manager(session);
	close(session->sock);
	return (SUCCESS);
}
