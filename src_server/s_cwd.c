/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_cwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 14:52:22 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 15:15:04 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

static int			check_cdrequest(t_session *session, char **cd)
{
	char			dir[256];

	if (!session->argv[1])
	{
		send_msg(session->cs, 1, "451 No action taken. Dir not specified.\r\n");
		return (FAILURE);
	}
	if (!(*cd = getcwd(dir, 255)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted."
				" Local error in processing. \r\n");
		return (FAILURE);
	}
	if ((ft_strcmp(*cd, session->run_dir) == 0 &&
		ft_strcmp(session->argv[1], "..") == 0) ||
		ft_strncmp(session->argv[1], "/", 1) == 0 ||
		ft_strncmp(session->argv[1], "~/", 2) == 0)
	{
		send_msg(session->cs, 1, "550 Requested action not taken.\r\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int					s_cwd(t_session *session)
{
	char			*cd;

	if (check_cdrequest(session, &cd) == FAILURE)
		return (FAILURE);
	cd = session->argv[1] ? session->argv[1] : session->run_dir;
	if (chdir(cd) == -1)
	{
		send_msg(session->cs, 1, "550 Requested action not taken."
				" File or directory not found.\r\n");
		return (SUCCESS);
	}
	s_pwd(session);
	return (SUCCESS);
}
