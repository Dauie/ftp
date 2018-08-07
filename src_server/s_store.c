#include "../incl/server.h"

int 	s_store(t_session *session)
{
	off_t len;

	if (session->mode == M_PSV)
	{
		if (!session->argv[1])
		{
			close_passive(session, T_SVR);
			send_msg(session->cs, 1, "451 Requested action aborted. File not specified. \r\n");
			return (FAILURE);
		}
		if ((session->fd = open(session->argv[1], O_CREAT | O_RDWR, 0755)) == -1)
		{
			close_passive(session, T_SVR);
			dprintf(STDERR_FILENO, "[-]Error creating %s.", session->argv[1]);
			return (FAILURE);
		}
		if ((recv_msg(session->psv->cs, session->psv->buff, &session->psv->run)) == FAILURE)
		{
			close_passive(session, T_SVR);
			send_msg(session->cs, 1, "550 Requested action not taken. File unavailable. \r\n");
			return (FAILURE);
		}
		len = ft_atoi(session->psv->buff);
		if ((recv_file(session->psv->cs, session->fd, session->psv->buff, len)) == FAILURE)
		{
			send_msg(session->cs, 1, "550 Closing data connection. Requested file action unsuccessful. \r\n");
			dprintf(STDERR_FILENO, "[-]Error receiving %s from client.",
					session->argv[1]);
		}
		else
			send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful. \r\n");
		close_passive(session, T_SVR);
		close(session->fd);
		return (SUCCESS);
	}
	send_msg(session->cs, 1, "425 Can't open data connection. Use PASV. \r\n");
	return (SUCCESS);
}
