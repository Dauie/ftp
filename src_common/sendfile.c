#include "../incl/ftp.h"

int			send_file(int sock, int fd, char *buff, off_t len)
{
	off_t 	rd;
	ssize_t ret;

	rd = 0;
	while (rd < len)
	{
		ft_bzero(buff, BUFFSZ);
		if ((ret = read(fd, buff, BUFFSZ)) == -1){
			printf("Issue reading file\n");
			return (EXIT_FAILURE);
		}
		if (ret == 0) {
			printf("EOF\n");
			return (EXIT_SUCCESS);
		}
		rd += BUFFSZ;
		if ((send(sock, buff, ft_strlen(buff), MSG_DONTWAIT) == -1))
		{
			printf("Failed to send file\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int		send_msg(int sock, int n, ...)
{
	va_list	ap;
	char	**tmp;
	char	buff[BUFFSZ];
	int		i;

	i = -1;
	printf("Enter send\n");
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

