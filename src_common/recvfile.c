#include "../incl/ftp.h"


int 		recv_msg(t_session *session)
{
	ssize_t ret;
	if ((ret = recv(session->cs, session->buff, BUFFSZ - 1, 0)) == -1)
		return (EXIT_FAILURE);
	else if (ret == 0)
		session->run = FALSE;
	write(STDOUT_FILENO, session->buff, BUFFSZ);
	return (EXIT_SUCCESS);
}

int 	recv_file(t_session *session)
{
	ssize_t ret;
	t_session *s;

	s = session->mode == M_PSV ? session->psv : session;
	ret = TRUE;
	while (ret >= TRUE)
	{
		if ((ret = recv(s->sock, s->buff, BUFFSZ, MSG_DONTWAIT)) == -1)
		{
			printf("[-]Error receiving data from \n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

