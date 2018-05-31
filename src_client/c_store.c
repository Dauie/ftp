#include "../incl/client.h"

int 	c_store(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	write(1, session->buff, ft_strlen(session->buff));
	return (EXIT_SUCCESS);
}