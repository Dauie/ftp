#include "../incl/client.h"

int 	c_dircmd(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	return (SUCCESS);
}

int 	c_list(t_session *session)
{
	off_t ret;

	if (session->mode == M_PSV)
	{
		ft_bzero(session->psv->buff, BUFF_SIZE);
		while (recv(session->psv->sock, session->psv->buff, BUFFSZ, 0) > 0)
		{
			write(1, session->psv->buff, BUFFSZ);
			ft_bzero(session->psv->buff, BUFF_SIZE);
		}
		ret = recv_msg(session->sock, session->buff, &session->run);
		close_passive(session, T_CLI);
		return ((int)ret);
	}
	recv_msg(session->sock, session->buff, &session->run);
	return (FAILURE);
}
