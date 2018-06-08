#include "../incl/server.h"

int 	s_store(t_session *session)
{
	off_t len;
	if (session->mode != M_PSV || accept_connection(session->psv) == EXIT_FAILURE ||
		session->csin.sin_addr.s_addr != session->psv->sin.sin_addr.s_addr)
	{
		send_msg(session->cs, 1, "425 Can't open data connection.\r\n");
		return (EXIT_FAILURE);
	}
	if (!session->argv[1] || (session->fd = open(session->argv[1], O_RDONLY) < 0))
	{
		send_msg(session->cs, 1, "550 Requested action not taken. File unavailable.\r\n");
		return (EXIT_FAILURE);
	}
	len = lseek(session->fd, 0, SEEK_END);
	if (send_file(session->psv->sock, session->fd, session->psv->buff, len) ==  EXIT_FAILURE)
	{
		send_msg(session->cs, 1, "550 Requested action not taken. File unavailable.\r\n");
		return (EXIT_FAILURE);
	}
	send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful.\r\n");
	close(session->psv->sock);
	close(session->fd);
	session->mode = M_NON;
	return (EXIT_SUCCESS);
}