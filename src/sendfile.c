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
	return (EXIT_SUCCESS);
}

int			ftp_sendfile(int fd, int sock, off_t *offset, off_t len)
{
	char 	buff[BUFFSZ];
	size_t 	send_size;
	int 	ret;

	ft_bzero(&buff, BUFFSZ);
	// Check how many bytes still need to be sent
	send_size = len - *offset > BUFFSZ ? len - *offset : BUFFSZ;
	add_header(send_size, &buff);
	if ((ret = read(fd, &buff + HDRSZ, send_size - HDRSZ)) == -1)
	{
		printf("[-]Error reading from temp send file\n");
		return(EXIT_FAIL);
	}
	*offset += ret - HDRSZ;
	if ((ret = send(sock, buff, BUFFSZ, 0)) == -1)
	{
		printf("[-]Error sending data to client on socket %d\n", sock);
		return(EXIT_FAIL);
	}
	// figure out how to manage returns here properly to stop the transmission
	return (EXIT_SUCCESS);
}