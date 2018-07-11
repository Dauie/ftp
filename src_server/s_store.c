#include "../incl/server.h"

int 	s_store(t_session *session)
{
	off_t len;

	if (session->mode == M_PSV)
	{
		session->fd = open(session->argv[1], O_CREAT | O_RDWR);
		if ((recv_msg(session->psv->cs, session->psv->buff, &session->psv->run)) == FAILURE)
		{
			close_passive(session, T_SVR);
			send_msg(session->cs, 1, "550 Requested action not taken. File unavailable. \r\n");
			return (FAILURE);

		}
		len = ft_atoi(session->psv->buff);
		if ((recv_file(session->psv->cs, session->fd, session->psv->buff, len)) == FAILURE)
			printf("[-]Error receiving %s from client.", session->argv[1]);
		send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful. \r\n");
		close_passive(session, T_SVR);
		close(session->fd);
		return (SUCCESS);
	}
	send_msg(session->cs, 1, "425 Can't open data connection. Use PASV. \r\n");
	return (SUCCESS);
}
