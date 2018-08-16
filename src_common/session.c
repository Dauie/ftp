/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:26 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 15:28:29 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

int			init_session(t_session *session, char **av, char **env)
{
	char	*start_path;

	session->mode = M_NON;
	session->run = TRUE;
	if (!env || !(session->env = ft_tbldup(env, ft_tbllen(env))))
		return (FAILURE);
	if (av[1])
		session->port = ft_atoi(av[1]);
	if (!(start_path = ft_getenvar(session->env, "PWD", 3)))
	{
		dprintf(STDERR_FILENO, "[-]Error setting server path\n");
		exit(FAILURE);
	}
	ft_strcpy(session->run_dir, &start_path[4]);
	free(start_path);
	return (SUCCESS);
}

void		clean_session(t_session *session)
{
	if (session->argv)
	{
		ft_tbldel(&session->argv);
		session->argv = NULL;
	}
	if (session->psv && session->mode == M_NON)
	{
		if (session->psv->argv)
			ft_tbldel(&session->psv->argv);
		free(session->psv);
		session->psv = NULL;
	}
}

void		destroy_session(t_session *session)
{
	if (session->env)
		ft_tbldel(&session->env);
	if (session->argv)
		ft_tbldel(&session->argv);
	if (session->psv)
	{
		if (session->psv->argv)
			ft_tbldel(&session->psv->argv);
		free(session->psv);
	}
	close(session->sock);
}