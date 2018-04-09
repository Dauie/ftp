#include "../incl/ftp.h"

int			ftp_sendfile(int fd, int sock, off_t *offset, off_t len)
{
	char 	buff[BUFFSZ + 1];
	size_t 	send_size;
	size_t 	ret;

	ret = 0;
	if ((send_size = len - *offset) > BUFFSZ)
		send_size = BUFFSZ;
	if (ret = read(fd, &buff, send_size) == -1)
	{
		printf("[-]Error reading from temp send file\n");
		return(EXIT_FAIL);
	}
	buff[ret] = '\0';
	offset += ret;
	if (ret = (send(sock, buff, ret, 0)) == -1)
	{
		printf("[-]Error sending data to client on socket %d\n", sock);
		return(EXIT_FAIL);
	}
	return (ret);
}