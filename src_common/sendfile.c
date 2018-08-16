/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:17 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 12:14:32 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

off_t		get_file_size(char *file)
{
	off_t	len;
	int		fd;

	fd = open(file, O_RDONLY);
	if ((len = lseek(fd, 0, SEEK_END)) == -1)
		return (FAILURE);
	close(fd);
	return (len);
}

void		close_passive(t_session *session, t_type type)
{
	if (type == T_SVR && session->psv->cs > 2)
		close(session->psv->cs);
	else if (type == T_CLI && session->psv->port > 2)
		close(session->psv->port);
	session->mode = M_NON;
}

int			send_file(int sock, int fd, char *buff, off_t len)
{
	off_t	rd;
	ssize_t	ret;

	rd = 0;
	while (rd < len)
	{
		ft_bzero(buff, BUFFSZ);
		if ((ret = read(fd, buff, BUFFSZ)) == -1)
		{
			printf("[-]Issue reading file\n");
			return (FAILURE);
		}
		if (ret == 0)
			return (SUCCESS);
		rd += ret;
		if ((send(sock, buff, (size_t)ret, MSG_WAITALL) == -1))
		{
			printf("[-]Failed to send file\n");
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

int			send_msg(int sock, int n, ...)
{
	va_list	ap;
	char	**tmp;
	char	buff[BUFFSZ];
	int		i;

	i = -1;
	va_start(ap, n);
	ft_bzero(buff, BUFFSZ);
	if (!(tmp = ft_memalloc(sizeof(char *) * (n + 1))))
		return (FAILURE);
	while (++i < n)
		tmp[i] = va_arg(ap, char *);
	i = -1;
	while (++i < n)
		ft_strcat(buff, tmp[i]);
	free(tmp);
	if (send(sock, buff, BUFFSZ, MSG_WAITALL) == -1)
	{
		printf("[-]Error writing to socket\n");
		return (FAILURE);
	}
	return (SUCCESS);
}
