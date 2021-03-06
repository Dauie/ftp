/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recvfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:07 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/16 12:57:46 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

int			recv_msg(int sock, char *buff, int *run)
{
	ssize_t	ret;
	char	*nl;

	ft_bzero(buff, BUFFSZ);
	if ((ret = recv(sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
		return (FAILURE);
	else if (ret == 0)
		*run = FALSE;
	if (buff[0])
	{
		if ((nl = ft_strrchr(buff, '\n')) != NULL)
			*nl = '\0';
		printf("[*]%s\n", buff);
	}
	return (SUCCESS);
}

int			recv_msg_svr(int sock, char *buff, int *run, t_session *session)
{
	ssize_t	ret;
	char	*nl;

	ft_bzero(buff, BUFFSZ);
	if ((ret = recv(sock, buff, BUFFSZ, MSG_WAITALL)) == -1)
		return (FAILURE);
	else if (ret == 0)
		*run = FALSE;
	if (buff[0])
	{
		if ((nl = ft_strrchr(buff, '\n')) != NULL)
			*nl = '\0';
		printf("[*]'%s' from %s\n", buff, inet_ntoa(session->sin.sin_addr));
	}
	return (SUCCESS);
}

int			recv_file(int sock, int fd, char *buff, off_t len)
{
	off_t	read;
	off_t	ret;
	off_t	curread;

	read = 0;
	while (read < len)
	{
		ft_bzero(buff, BUFFSZ);
		curread = len - read < BUFFSZ ? len - read : BUFFSZ;
		if ((ret = recv(sock, buff, (size_t)curread, MSG_WAITALL)) == -1)
			return (FAILURE);
		read += ret;
		if ((write(fd, buff, (size_t)ret)) == -1)
		{
			printf("Error writing to new file\n");
			return (FAILURE);
		}
	}
	return (SUCCESS);
}
