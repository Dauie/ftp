#include "../incl/client.h"

int 	c_store(t_session *session)
{
	off_t   len;
	char    *lstr;

	if (session->mode == M_PSV)
	{
		len = session->argv[1] ? get_file_size(session->argv[1]) : 0;
		if (!session->argv[1] || (session->fd = open(session->argv[1], O_RDONLY)) < 0)
		{
			close_passive(session, T_CLI);
			recv_msg(session->sock, session->buff, &session->run);
			printf("[-]Error reading from passive socket.\n");
			return (FAILURE);
		}
		send_msg(session->psv->sock, 2, (lstr = ft_itoa(len)), " \r\n");
		free(lstr);
		send_file(session->psv->sock, session->fd, session->psv->buff, len);
		recv_msg(session->sock, session->buff, &session->run);
		close_passive(session, T_CLI);
		close(session->fd);
		return (SUCCESS);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (FAILURE);
}
