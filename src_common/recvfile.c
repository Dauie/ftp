#include "../incl/ftp.h"


int 		recv_msg(int sock, char *buff, int *run)
{
	ssize_t ret;

	if ((ret = recv(sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
		return (EXIT_FAILURE);
	else if (ret == 0)
		*run = FALSE;
	ft_printf("[*]%s", buff);
    if (ft_strchr(buff, '\n') == NULL)
        write(1, "\n", 1);
	return (EXIT_SUCCESS);
}

int		recv_file(int sock, int fd, char *buff, off_t len)
{
	off_t rd;
    off_t ret;

    rd = 0;
    ft_bzero(buff, BUFFSZ);
	while (rd < len)
	{
        ft_bzero(buff, BUFFSZ);
        if ((ret = recv(sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
            return (EXIT_FAILURE);
        rd += BUFFSZ;
		if ((write(fd, buff, ft_strlen(buff))) == -1)
		{
			printf("Error writing to new file\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

