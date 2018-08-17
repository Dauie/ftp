/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:35 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 18:20:49 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

void		grim_reaper(int sig)
{
	int		saverno;

	(void)sig;
	saverno = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		continue ;
	errno = saverno;
}

void		handle_killsvr_sig(int sig)
{
	(void)sig;
	if (g_session->argv)
		ft_tbldel(&g_session->argv);
	if (g_session->env)
		ft_tbldel(&g_session->env);
	if (g_session->psv)
	{
		if (g_session->psv->port > 2)
			close(g_session->psv->port);
		if (g_session->psv->fd > 2)
			close(g_session->psv->fd);
		free(g_session->psv);
	}
	close(g_session->sock);
	free(g_session);
	ft_putstr("\n[+]FTP Server Terminated - (^-^)^ bye.\n");
	exit(1);
}

void		handle_killcli_sig(int sig)
{
	(void)sig;
	send_msg(g_session->sock, 1, "QUIT");
	if (g_session->argv)
		ft_tbldel(&g_session->argv);
	if (g_session->psv)
	{
		if (g_session->psv->port > 2)
			close(g_session->psv->port);
		if (g_session->psv->fd > 2)
			close(g_session->psv->fd);
		free(g_session->psv);
	}
	close(g_session->sock);
	free(g_session);
	printf("\n[+](^-^)^ bye.\n");
	exit(1);
}
