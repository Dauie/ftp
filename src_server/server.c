/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:45:45 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 15:33:30 by rlutt            ###   ########.fr       */
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
			close(session->cs);
		else if (session->pid > 0)
			close(session->cs);
		else if (session->pid == 0)
		{
			close(session->sock);
			manage_client_session(session);
		}
	}
}

int				accept_connection(t_session *session)
{
	uint		cslen;

	cslen = sizeof(struct sockaddr_in);
	if (!(session->cs = accept(session->sock,
					(struct sockaddr*)&session->sin, &cslen)))
		return (FAILURE);
	printf("[+]New connection %s:%d\n",
		inet_ntoa(session->sin.sin_addr),
		ntohs(session->sin.sin_port));
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

int				main(int ac, char **av)
{
	t_session	*session;
	extern char	**environ;

	if (ac != 2)
		usage(av[0]);
	signal(SIGINT, handle_killsvr_sig);
	signal(SIGCHLD, grim_reaper);
	if (!(session = ft_memalloc(sizeof(t_session))))
		return (FAILURE);
	g_session = session;
	if (init_session(session, av, environ) == FAILURE)
		return (FAILURE);
	if (create_endpoint(session, NULL) == FAILURE)
		return (FAILURE);
	printf("[+]Server started on port %d\n", session->port);
	session_manager(session);
	destroy_session(session);
	return (SUCCESS);
}
