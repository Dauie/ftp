#include "../incl/client.h"

int 	c_retrieve(t_session *session)
{
	off_t len;

	if (session->mode == M_PSV)
	{
		if (!session->argv[1])
		{
			close_passive(session, T_SVR);
			dprintf(STDERR_FILENO, "[-]Error file not specified.\n");
			return (FAILURE);
		}
		if ((session->fd = open(session->argv[1], O_CREAT | O_RDWR, 0755)) == -1)
		{
			close_passive(session, T_CLI);
			dprintf(STDERR_FILENO, "[-]Error creating file.");
			return (FAILURE);
		}
		if ((recv_msg(session->psv->sock, session->psv->buff, &session->psv->run)) == FAILURE)
		{
			close_passive(session, T_CLI);
			dprintf(STDERR_FILENO, "[-]Error reading from passive socket.\n");
			return (FAILURE);
		}
		len = ft_atoi(session->psv->buff);
		if ((recv_file(session->psv->sock, session->fd, session->psv->buff, len)) == FAILURE)
		{
			close_passive(session, T_CLI);
			dprintf(STDERR_FILENO, "[-]Error receiving %s from server.",
					session->argv[1]);
			return (FAILURE);
		}
		close_passive(session, T_CLI);
		close(session->fd);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (SUCCESS);
}
