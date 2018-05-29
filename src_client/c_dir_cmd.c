#include "../incl/client.h"

int 	c_cwd(t_session *session)
{
	recv_msg(session);
	write(1, session->buff, ft_strlen(session->buff));
	return (EXIT_SUCCESS);
}

int		c_pwd(t_session *session)
{
	recv_msg(session);
	write(1, session->buff, ft_strlen(session->buff));
	return (EXIT_SUCCESS);
}

int 	c_list(t_session *session)
{
	recv_msg(session);
	write(1, session->buff, ft_strlen(session->buff));
	return (EXIT_SUCCESS);
}