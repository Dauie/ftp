#include "../incl/ftp.h"


int 		recv_msg(int sock, char *buff, int *run)
{
	ssize_t ret;
	if ((ret = recv(sock, buff, BUFFSZ - 1, MSG_WAITALL)) == -1)
		return (EXIT_FAILURE);
	else if (ret == 0)
		*run = FALSE;
	printf("ret= %d\n", (int)ret);
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
	}
	return (EXIT_SUCCESS);
}

