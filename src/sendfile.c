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

	ret = 0;
	ft_bzero(&buff, BUFFSZ);
	send_size = len - *offset > BUFFSZ ? len - *offset : BUFFSZ;
	if ((ret = read(fd, &buff, send_size)) == -1)
	{
		printf("[-]Error reading from temp send file\n");
		return(EXIT_FAIL);
	}
	offset += ret;
	if ((ret = send(sock, buff, BUFFSZ, 0)) == -1)
	{
		printf("[-]Error sending data to client on socket %d\n", sock);
		return(EXIT_FAIL);
	}
	return (ret);
}
