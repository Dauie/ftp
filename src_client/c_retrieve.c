#include "../incl/client.h"

int 	c_retrieve(t_session *session)
{
	recv_msg(session);
	write(1, session->buff, ft_strlen(session->buff));
	return (EXIT_SUCCESS);
}