#include "../incl/client.h"

int 	c_passive(t_session *session)
{
	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (EXIT_FAILURE);
	init_session(session->psv);
	recv_msg();
}