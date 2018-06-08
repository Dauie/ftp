#include "../incl/client.h"

int 	c_retrieve(t_session *session)
{
	session->fd = open(ft_strjoin(session->argv[1], ".trans"), O_CREAT | O_RDWR);
//	if ((recv_msg(session->psv->sock, session->psv->buff, &session->psv->run)) == EXIT_FAILURE)
//		printf("[-]Error reading from passive socket - Err %s\n", strerror(errno));
	recv_file(session->psv->sock, session->fd, session->psv->buff);
	recv_msg(session->sock, session->buff, &session->run);
	write(1, session->buff, BUFFSZ);
	return (EXIT_SUCCESS);
}