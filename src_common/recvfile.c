/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recvfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:07 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 13:44:07 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

int			recv_msg(int sock, char *buff, int *run)
{
	ssize_t	ret;

	ft_bzero(buff, BUFFSZ);
	if ((ret = recv(sock, buff, BUFFSZ, MSG_PEEK)) == -1)
		return (FAILURE);
	if ((ret = recv(sock, buff, (size_t)ret, MSG_WAITALL)) == -1)
		return (FAILURE);
	else if (ret == 0)
		*run = FALSE;
	if (buff[0])
	{
		ft_printf("[*]%s", buff);
		if (ft_strchr(buff, '\n') == NULL)
			write(1, "\n", 1);
	}
	return (SUCCESS);
}

int			recv_file(int sock, int fd, char *buff, off_t len)
{
	off_t	rd;
	off_t	ret;

	rd = 0;
	while (rd < len)
	{
		ft_bzero(buff, BUFFSZ);
		if ((ret = recv(sock, buff, BUFFSZ, MSG_PEEK)) == -1)
			return (FAILURE);
		if ((ret = recv(sock, buff, (size_t)ret, MSG_WAITALL)) == -1)
			return (FAILURE);
		rd += ret;
		if ((write(fd, buff, (size_t)ret)) == -1)
		{
			printf("Error writing to new file\n");
			return (FAILURE);
		}
	}
	return (SUCCESS);
}
