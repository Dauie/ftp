#include "../incl/ftp.h"

int 		send_ok(t_session *session)
{
	char buff[BUFFSZ];

	ft_strcpy(buff, "OK");
	// Send OK message to server, verifying space if avail.
	if (send(session->sock, buff, BUFFSZ, MSG_WAITALL) == -1)
		return (EXIT_FAIL);
	ft_printf("sent OK msg\n");
	return (EXIT_SUCCESS);
}

int			prep_recv(t_session *session)
{
	session->off =  0;
	session->size = 0;
	ft_bzero(session->buff, BUFFSZ);
	// Wait for size of response being sent
	recv(session->sock, session->buff, BUFFSZ, MSG_WAITALL);
	ft_printf("Got file size%s\n", session->buff);
	// need to do error checking to check OS for sufficient space.
	send_ok(session);
	session->size = ft_atoi(session->buff);
	ft_printf("%i", (int)session->size);
	return (EXIT_SUCCESS);
}

int 	ftp_recvfile(t_session *session)
{
	char 	buff[BUFFSZ];
	ssize_t ret;

	ret = 0;
	prep_recv(session);
	while (session->off < session->size)
	{
		if ((ret = recv(session->sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
		{
			printf("[-]Error receiving data from \n");
			return (EXIT_FAIL);
		}
		write(session->fd, session->buff, BUFFSZ);
		session->off += ret;
	}
	return (EXIT_SUCCESS);
}

