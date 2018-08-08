/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_store.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:45:35 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 22:32:47 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

static int			prep_sstore(t_session *session)
{

	if (!session->argv[1])
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "550 Closing data connection."
				" File not specified. \r\n");
		return (FAILURE);
	}
	if ((session->fd = open(session->argv[1], O_CREAT | O_RDWR, 0755)) == -1)
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "451 Closing data connection."
				" Error in local processing. \r\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

static off_t			recieve_file_size(t_session *session)
{
	off_t	len;

	if ((recv_msg(session->psv->cs, session->psv->buff,
				  &session->psv->run)) == FAILURE)
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "550 Closing data connection."
				" File unavailable. \r\n");
		return (FAILURE);
	}
	if ((len = ft_atoi(session->psv->buff)) <= 0)
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "550 Closing data connection."
				"Requested file action unsuccessful. \r\n");
		return (FAILURE);
	}
	return (len);
}

int			s_store(t_session *session)
{
	off_t	len;

	if (session->mode == M_PSV)
	{
		if ((len = recieve_file_size(session)) == FAILURE)
			return (FAILURE);
		if (prep_sstore(session) == FAILURE)
			return (FAILURE);
		if ((recv_file(session->psv->cs, session->fd,
				session->psv->buff, len)) == FAILURE)
			send_msg(session->cs, 1, "550 Closing data connection."
					"Requested file action unsuccessful. \r\n");
		else
			send_msg(session->cs, 1, "226 Closing data connection."
					" Requested file action successful. \r\n");
		close_passive(session, T_SVR);
		close(session->fd);
		return (SUCCESS);
	}
	send_msg(session->cs, 1, "425 Can't open data connection. Use PASV. \r\n");
	return (SUCCESS);
}
