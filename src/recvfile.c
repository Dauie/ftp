#include "../incl/ftp.h"



// You need to peed the stream, find out how many bytes are being send, read that
// many bytes, write them to the temp file, and send a confirmation of how many bytes were sent.
static int		peek_header(int sock)
{
	sock = 2;
	return(sock);
}


int 	ftp_recvfile(int fd, int sock, off_t *offset, off_t size)
{
	size_t 	read_size;
	char 	buff[BUFFSZ];
	int 	ret;

	read_size = peek_header(sock);
	if ((ret = recv(sock, buff, read_size, MSG_WAITALL)) ==  -1)
		return (EXIT_FAIL);
	*offset += ret;
	if ((write(fd, &buff + 10, ret)) == -1)
	{
		printf("[-]Issue writing to recv temp file on fd %d\n", fd);
		return (EXIT_FAIL);
	}
	add_header(read_size, buff);
	if ((ret = send(sock, buff, 10, 0)) == -1)
	{
		printf("[-]Error sending data to client on socket %d\n", sock);
		return(EXIT_FAIL);
	}
	return (size - *offset);
}
