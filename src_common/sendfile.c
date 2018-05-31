#include "../incl/ftp.h"

int			send_file(int sock, int fd)
{
	close(fd);
	send_msg(sock, 1, "502 [-]Command not implemented.\n\r");
	send_msg(sock, 1, "226 [+]Closing data connection.\n\r");
	return (EXIT_SUCCESS);
}

int		send_msg(int sock, int n, ...)
{
	va_list	ap;
	char	**tmp;
	char	buff[BUFFSZ];
	int		i;
	ssize_t ret;

	i = -1;
	va_start(ap, n);
	ft_bzero(buff, BUFFSZ);
	if (!(tmp = ft_memalloc(sizeof(char *) * (n + 1))))
		return (EXIT_FAILURE);
	while (++i < n)
		tmp[i] = va_arg(ap, char *);
	i = -1;
	while (++i < n)
		ft_strcat(buff, tmp[i]);
	free(tmp);
	ret = send(sock, buff, BUFFSZ, MSG_WAITALL);
	printf("sent: %d\n", (int)ret);
	return (EXIT_SUCCESS);
}

