#include "../incl/client.h"

int 	c_cwd(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	write(1, session->buff, BUFFSZ);
	return (EXIT_SUCCESS);
}

int		c_pwd(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	write(1, session->buff, BUFFSZ);
	return (EXIT_SUCCESS);
}

int 	c_list(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	write(1, session->buff, BUFFSZ);
	return (EXIT_SUCCESS);
}