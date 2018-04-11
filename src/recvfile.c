#include "../incl/ftp.h"

int 	ftp_recvfile(int fd, int sock, off_t *offset)
{
	char 	buff[BUFFSZ];
	int 	ret;

	ret = 0;
	if ((ret = recv(sock, buff, BUFFSZ, MSG_WAITALL)) ==  -1)
		return (EXIT_FAIL);
	*offset += ret;
	if ((write(fd, buff, ret)) == -1)
	{
		printf("[-]Issue writing to recv temp file on fd %d\n", fd);
		return (EXIT_FAIL);
	}
	return (ret);
}
