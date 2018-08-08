/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_retrieve.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:45:25 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 16:42:55 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

static int			is_file(char *pwd, char *file)
{
	struct stat		sd;
	char			*buff;

	if (!(buff = ft_strjoinn(3, pwd, "/", file)))
		return (FAILURE);
	if (stat(buff, &sd) == -1 || (S_ISREG(sd.st_mode) == 0))
	{
		free(buff);
		return (FAILURE);
	}
	free(buff);
	return (SUCCESS);
}

static int			check_requested_file(t_session *session)
{
	char			dir[256];
	char			*str;

	if (!session->argv[1])
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "451 Request aborted. No file specified.\r\n");
		return (FAILURE);
	}
	if (!(str = getcwd(dir, 255)))
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "451 Request aborted. Err in processing.\r\n");
		return (FAILURE);
	}
	if (is_file(str, session->argv[1]) == FAILURE)
	{
		free(str);
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "550 Action not taken. No directories.\r\n");
		return (FAILURE);
	}
	free(str);
	return (SUCCESS);
}

static off_t		prepare_retrieve(t_session *session)
{
	char			*lstr;

	if (check_requested_file(session) == FAILURE)
		return (FAILURE);
	if ((session->fd = open(session->argv[1], O_RDONLY)) < 0)
	{
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "550 Action not taken. File unavailable.\r\n");
		return (FAILURE);
	}
	send_msg(session->psv->cs, 1,
			(lstr = ft_itoa(get_file_size(session->argv[1]))));
	if (lstr)
		free(lstr);
	return (get_file_size(session->argv[1]));
}

int					s_retrieve(t_session *session)
{
	off_t			len;

	if (session->mode == M_PSV)
	{
		if ((len = prepare_retrieve(session)) == FAILURE)
			return (FAILURE);
		if (send_file(session->psv->cs, session->fd,
					session->psv->buff, len) ==  FAILURE)
		{
			close_passive(session, T_SVR);
			close(session->fd);
			send_msg(session->cs, 1, "451 Action not taken. Err in trans.\r\n");
			return (FAILURE);
		}
		send_msg(session->cs, 1, "226 Closing data connection."
				" Requested file action successful.\r\n");
		close_passive(session, T_SVR);
		close(session->fd);
		return (SUCCESS);
	}
	send_msg(session->cs, 1, "425 Can't open data connection. Use PASV.\r\n");
	return (FAILURE);
}
