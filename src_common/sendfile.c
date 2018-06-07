#include "../incl/ftp.h"

int			send_file(int sock, int fd)
{
	(void)sock;
	(void)fd;
	return (EXIT_SUCCESS);
}

int		send_msg(int sock, int n, ...)
{
	va_list	ap;
	char	**tmp;
	char	buff[BUFFSZ];
	int		i;

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
	if (send(sock, buff, BUFFSZ, MSG_WAITALL) == -1) {
		printf("[-]Error writing to socket\n");
		return (EXIT_FAILURE);
	}
	write(1, "sent\n", 5);
	return (EXIT_SUCCESS);
}

