/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_retrieve.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:43:45 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 14:03:25 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/client.h"

static int	prep_cretrieve(t_session *session, off_t *len)
{
	if (!session->argv[1])
	{
		close_passive(session, T_CLI);
		dprintf(STDERR_FILENO, "[-]Error file not specified.\n");
		return (FAILURE);
	}
	if ((session->fd = open(session->argv[1], O_CREAT | O_RDWR, 0755)) == -1)
	{
		close_passive(session, T_CLI);
		dprintf(STDERR_FILENO, "[-]Error creating file.\n");
		return (FAILURE);
	}
	if ((recv_msg(session->psv->sock, session->psv->buff,
			&session->psv->run)) == FAILURE)
	{
		close_passive(session, T_CLI);
		dprintf(STDERR_FILENO, "[-]Error reading from passive socket.\n");
		return (FAILURE);
	}
	*len = ft_atoi(session->psv->buff);
	return (SUCCESS);
}

int			c_retrieve(t_session *session)
{
	off_t	len;

	if (session->mode == M_PSV)
	{
		if (prep_cretrieve(session, &len) == FAILURE)
			return (FAILURE);
		if ((recv_file(session->psv->sock, session->fd,
				session->psv->buff, len)) == FAILURE)
		{
			close_passive(session, T_CLI);
			dprintf(STDERR_FILENO, "[-]Error receiving %s from server.\n",
					session->argv[1]);
			return (FAILURE);
		}
		close_passive(session, T_CLI);
		close(session->fd);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (SUCCESS);
}
