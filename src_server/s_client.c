/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_client.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:53 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/15 11:28:07 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

int		(*g_funcs[])(t_session *) = {&s_cwd, &s_help, &s_list,
	&s_passive, &s_pwd, &s_quit, &s_retrieve, &s_store};

char	*g_cmd[] = {"CWD", "HELP", "LIST", "PASV", "PWD",
	"QUIT", "RETR", "STOR"};

int				s_quit(t_session *session)
{
	if (ft_strcmp((const char *)&session->buff, "quit") == 0)
	{
		printf("[+]Host has disconnected from socket %d\n", session->cs);
		close(session->cs);
		session->run = FALSE;
	}
	return (SUCCESS);
}

static void		dispatch_command(t_session *session)
{
	int			i;
	int			len;

	i = -1;
	len = (int)ft_tbllen(g_cmd);
	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return ;
	while (session->argv[0] && ++i < len)
	{
		if (ft_strncmp(session->argv[0],
					g_cmd[i], ft_strlen(g_cmd[i])) == 0)
		{
			g_funcs[i](session);
			break ;
		}
	}
	if (i == len)
		send_msg(session->cs, 1, "502 Command not implemented. \r\n");
}

void			manage_client_session(t_session *session)
{
	while (session->run)
	{
		ft_bzero(session->buff, BUFFSZ);
		if (recv_msg_svr(session->cs, session->buff,
						&session->run, session) == FAILURE)
			continue ;
		dispatch_command(session);
		clean_session(session);
	}
	printf("[-]Client disconnected from %s\n",
		inet_ntoa(session->sin.sin_addr));
}
