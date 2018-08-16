/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:44:44 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/15 12:54:19 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

static char			*check_hostfile_entires(char *address)
{
	int				fd;
	char			*line;
	char			**entry;

	if ((fd = open("/etc/hosts", O_RDONLY)) < 0)
		return (NULL);
	while (gnl(fd, &line) > 0)
	{
		if (line[0] == '#' || !(entry = ft_strsplit(line, ' ')))
		{
			ft_strdel(&line);
			continue ;
		}
		if (entry[1] && ft_strcmp(entry[1], address) == 0)
		{
			ft_strdel(&line);
			line = ft_strdup(entry[0]);
			ft_tbldel(&entry);
			break ;
		}
		ft_strdel(&line);
		ft_tbldel(&entry);
	}
	close(fd);
	return (line);
}

int					create_socket(t_session *session, char *address)
{
	struct protoent *proto;
	char			*entry;

	entry = NULL;
	if ((proto = getprotobyname("tcp")) == 0)
		return (FAILURE);
	if ((session->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("[-]Error creating socket\n");
		return (FAILURE);
	}
	session->sin.sin_family = AF_INET;
	session->sin.sin_port = htons(session->port);
	if (address && (entry = check_hostfile_entires(address)) != NULL)
		address = entry;
	if (address)
		session->sin.sin_addr.s_addr = inet_addr(address);
	else
		session->sin.sin_addr.s_addr = htonl(INADDR_ANY);
	session->cslen = sizeof(session->csin);
	if (entry)
		free(entry);
	return (SUCCESS);
}

int					listen_socket(t_session *session)
{
	if ((listen(session->sock, 1)) == -1)
	{
		close(session->sock);
		return (FAILURE);
	}
	return (SUCCESS);
}

int					bind_socket(t_session *session)
{
	if (bind(session->sock, (const struct sockaddr *)&session->sin,
				sizeof(session->sin)) < 0)
	{
		printf("[-]Error binding socket on port %d\n", session->port);
		return (FAILURE);
	}
	return (SUCCESS);
}

int					options_socket(t_session *session)
{
	int				opt;

	opt = TRUE;
	if (setsockopt(session->sock, SOL_SOCKET, SO_REUSEADDR,
				(char *)&opt, sizeof(opt)) < 0)
	{
		printf("[-]Error setting options on port %d", session->port);
		return (FAILURE);
	}
	return (SUCCESS);
}
