#include "../incl/ftp.h"

int			prep_send(t_session *session)
{
	session->off =  0;
	session->size = 0;
	if ((session->size = lseek(session->fd, 0, SEEK_END)) == -1)
	{
		printf("[-]Error retrieving size of temp file\n");
		return(EXIT_FAIL);
	}
	add_header(session->size, session->buff);
	ft_printf("size of file from lseek %i - %s\n", session->size, session->buff);
	return (EXIT_SUCCESS);
}

int			sendfile(t_session *session)
{
	char 	buff[BUFFSZ];
	ssize_t ret;

	ret = 0;
	printf("got okay message\n");
	while (session->off < session->size)
	{
		if ((ret = read(session->fd, buff, BUFFSZ)) == -1)
		{
			printf("[-]Error reading from temp send file\n");
			return(EXIT_FAIL);
		}
		if ((send(session->sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
		{
			printf("[-]Error sending data to client on socket %d\n", session->sock);
			return(EXIT_FAIL);
		}
		printf("sent batch");
		session->off += ret;
	}
	return (EXIT_SUCCESS);
}

//int			ftp_sendfile(int fd, int sock, off_t *offset, off_t len)
//{
//	char 	buff[BUFFSZ];
//	ssize_t send_size;
//	ssize_t ret;
//
//	ft_bzero(&buff, BUFFSZ);
//	// Check how many bytes still need to be sent
//	send_size = len - *offset > BUFFSZ ? len - *offset : BUFFSZ;
//	add_header(send_size, buff);
//	if ((ret = read(fd, &buff + HDRSZ, send_size - HDRSZ)) == -1)
//	{
//		printf("[-]Error reading from temp send file\n");
//		return(EXIT_FAIL);
//	}
//	*offset += ret - HDRSZ;
//	if ((ret = send(sock, buff, BUFFSZ, 0)) == -1)
//	{
//		printf("[-]Error sending data to client on socket %d\n", sock);
//		return(EXIT_FAIL);
//	}
//	// figure out how to manage returns here properly to stop the transmission
//	return (EXIT_SUCCESS);
//}