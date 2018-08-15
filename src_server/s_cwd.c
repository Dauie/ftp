/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_cwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 14:52:22 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/14 16:56:29 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

static int			check_cdrequest(t_session *session,
									char **start_path, char **cd)
{
	char			dir[256];

	*start_path = NULL;
	if (!session->argv[1])
	{
		send_msg(session->cs, 1, "451 No action taken. Dir not specified.\r\n");
		return (FAILURE);
	}
	if (!(*cd = getcwd(dir, 255)) ||
		!(*start_path = ft_getenvar(session->env, "PWD", 3)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted."
				" Local error in processing. \r\n");
		return (FAILURE);
	}
	if ((ft_strcmp(*cd, &(*start_path)[4]) == 0 &&
		ft_strcmp(session->argv[1], "..") == 0) ||
		ft_strncmp(session->argv[1], "/", 1) == 0 ||
		ft_strncmp(session->argv[1], "~/", 2) == 0)
	{
		free(*start_path);
		send_msg(session->cs, 1, "550 Requested action not taken.\r\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int					s_cwd(t_session *session)
{
	char			*start_path;
	char			*cd;

	if (check_cdrequest(session, &start_path, &cd) == FAILURE)
		return (FAILURE);
	cd = session->argv[1] ? session->argv[1] : &start_path[4];
	if (chdir(cd) == -1)
	{
		free(start_path);
		send_msg(session->cs, 1, "550 Requested action not taken."
				" File or directory not found.\r\n");
		return (SUCCESS);
	}
	free(start_path);
	s_pwd(session);
	return (SUCCESS);
}
