#include "../incl/ftp.h"


int 		recv_msg(int sock, char *buff, int *run)
{
	ssize_t ret;
	printf("enter recv\n");
	if ((ret = recv(sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
		return (EXIT_FAILURE);
	else if (ret == 0)
		*run = FALSE;
	printf("recv: - %s\n", buff);
	return (EXIT_SUCCESS);
}

int		recv_file(int sock, int fd, char *buff)
{
	ssize_t ret;

	while ((ret = recv(sock, buff, BUFFSZ / 2, MSG_DONTWAIT)) != -1 && ret != 0)
	{
		ft_bzero(buff, BUFFSZ);
		if ((write(fd, buff, BUFFSZ)) == -1)
		{
			printf("Error writing to new file\n");
			return (EXIT_FAILURE);
		}

	}
	printf("recv msg ret - %zd - reason: %s\n", ret, strerror(errno));
	return (EXIT_SUCCESS);
}

