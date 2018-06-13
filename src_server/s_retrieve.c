#include "../incl/server.h"


int 	s_retrieve(t_session *session)
{
	off_t   len;
    char    *lstr;

	if (session->mode == M_PSV)
	{
        len = session->argv[1] ? get_file_size(session->argv[1]) : 0;
		if (!session->argv[1] || (session->fd = open(session->argv[1], O_RDONLY)) < 0)
		{
			close_passive(session);
			send_msg(session->cs, 1, "550 Requested action not taken. File unavailable.\r\n");
			return (EXIT_FAILURE);
		}
		send_msg(session->psv->cs, 1, (lstr = ft_itoa(len)));
		free(lstr);
		if (send_file(session->psv->cs, session->fd, session->psv->buff, len) ==  EXIT_FAILURE)
		{
			close_passive(session);
			send_msg(session->cs, 1, "550 Requested action not taken. File unavailable.\r\n");
			return (EXIT_FAILURE);
		}
		send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful.\r\n");
		close_passive(session);
		return (EXIT_SUCCESS);
	}
	send_msg(session->cs, 1, "425 Can't open data connection.\r\n");
	return (EXIT_FAILURE);
}

