/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_dir_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:43:12 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 21:53:54 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/client.h"

int			c_dircmd(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	return (SUCCESS);
}

int			c_list(t_session *session)
{
	off_t	ret;

	if (session->mode == M_PSV)
	{
		ft_bzero(session->psv->buff, BUFFSZ);
		while (recv(session->psv->sock, session->psv->buff, BUFFSZ, 0) > 0)
		{
			write(1, session->psv->buff, BUFFSZ);
			ft_bzero(session->psv->buff, BUFFSZ);
		}
		ret = recv_msg(session->sock, session->buff, &session->run);
		close_passive(session, T_CLI);
		return ((int)ret);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (SUCCESS);
}
