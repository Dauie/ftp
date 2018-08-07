#include "../incl/client.h"

int 	c_store(t_session *session)
{
	off_t   len;
	char    *lstr;

	if (session->mode == M_PSV)
	{
		if (!session->argv[1])
		{
			close_passive(session, T_SVR);
			dprintf(STDERR_FILENO, "[-]Error file not specified.\n");
			return (FAILURE);
		}
		len = get_file_size(session->argv[1]);
		if ((session->fd = open(session->argv[1], O_RDONLY)) < 0)
		{
			close_passive(session, T_CLI);
			dprintf(STDERR_FILENO, "[-]Error opening %s.\n", session->argv[1]);
			return (FAILURE);
		}
		send_msg(session->psv->sock, 2, (lstr = ft_itoa(len)), " \r\n");
		free(lstr);
		if (send_file(session->psv->sock, session->fd, session->psv->buff, len) == FAILURE)
			dprintf(STDERR_FILENO, "[-]Error sending %s", session->argv[1]);
		recv_msg(session->sock, session->buff, &session->run);
		close_passive(session, T_CLI);
		close(session->fd);
		return (SUCCESS);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (FAILURE);
}
