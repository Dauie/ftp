#include "../incl/ftp.h"

off_t       get_file_size(char *file)
{
    off_t   len;
	int 	fd;

    len = 0;
	fd = open(file, O_RDONLY);
    if ((len = lseek(fd, 0, SEEK_END)) == -1)
    {
        printf("%s\n", strerror(errno));
    }
    close(fd);
    return (len);
}

void        close_passive(t_session *session)
{
    close(session->psv->sock);
    close(session->fd);
    session->mode = M_NON;
}

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
		if (ret == 0)
			return (EXIT_SUCCESS);
		rd += BUFFSZ;
		if ((send(sock, buff, BUFFSZ, MSG_WAITALL) == -1))
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
	return (EXIT_SUCCESS);
}

