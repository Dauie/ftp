/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_store.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:43:58 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 22:49:30 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/client.h"

static int	prep_cstore(t_session *session, off_t *len)
{
	char	dir[PATH_MAX];

	if (!session->argv[1])
	{
		close_passive(session, T_CLI);
		dprintf(STDERR_FILENO, "[-]Error file not specified.\n");
	}
	if (is_file(getcwd(dir, PATH_MAX), session->argv[1]))
		*len = -1;
	else
		*len = get_file_size(session->argv[1]);
	if ((session->fd = open(session->argv[1], O_RDONLY)) < 0)
	{
		close_passive(session, T_CLI);
		dprintf(STDERR_FILENO, "[-]Error opening %s.\n", session->argv[1]);
	}
	return (SUCCESS);
}

int			c_store(t_session *session)
{
	off_t	len;
	char	*lstr;

	if (session->mode == M_PSV)
	{
		prep_cstore(session, &len);
		send_msg(session->psv->sock, 2, (lstr = ft_itoa(len)), " \r\n");
		free(lstr);
		if (send_file(session->psv->sock, session->fd,
				session->psv->buff, len) == FAILURE)
			dprintf(STDERR_FILENO, "[-]Error sending %s", session->argv[1]);
		recv_msg(session->sock, session->buff, &session->run);
		close_passive(session, T_CLI);
		close(session->fd);
		return (SUCCESS);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (FAILURE);
}
