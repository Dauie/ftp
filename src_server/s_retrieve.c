#include "../incl/server.h"

int 	s_retrieve(t_session *session)
{
	off_t len;

	if (session->mode != M_PSV)
	{
		send_msg(session->cs, 1, "425 Can't open data connection.\r\n");
		return (EXIT_FAILURE);
	}
	printf("Opening: %s\n", session->argv[1]);
	if (!session->argv[1] || (session->fd = open(ft_strjoin("./" ,session->argv[1]), O_RDONLY)) < 0)
	{
		printf("No file specified or could not open - %s", strerror(errno));
		send_msg(session->cs, 1, "550 Requested action not taken. File unavailable.\r\n");
		return (EXIT_FAILURE);
	}
	printf("fd for file: %d\n", session->fd);
	if ((len = lseek(session->fd, 0, SEEK_END)) == -1)
	{
		printf("%s\n", strerror(errno));
	}
	printf("file len: %d\n", (int)len);
//	send_msg(session->psv->cs, 1, "Hello?\r\n");
	if (send_file(session->psv->cs, session->fd, session->psv->buff, len) ==  EXIT_FAILURE)
	{
		send_msg(session->cs, 1, "550 Requested action not taken. File unavailable.\r\n");
		return (EXIT_FAILURE);
	}
	send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful.\r\n");
	close(session->psv->sock);
	close(session->fd);
	session->mode = M_NON;
	return (EXIT_SUCCESS);
}