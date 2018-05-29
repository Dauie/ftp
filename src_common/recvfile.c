#include "../incl/client.h"


int 		recv_msg(t_session *session)
{
	if (recv(session->cs, session->buff, BUFFSZ, 0) == -1)
		return (EXIT_FAILURE);
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
		if ((ret = recv(s->sock, s->buff, BUFFSZ, 0)) == -1)
		{
			printf("[-]Error receiving data from \n");
			return (EXIT_FAILURE);
		}
		write(s->fd, s->buff, ft_strlen(s->buff));
	}
	return (EXIT_SUCCESS);
}

