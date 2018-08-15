/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_passive.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:43:35 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 23:24:03 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/client.h"

static int		c_parse_port(char *response)
{
	char		**resp;
	char		**ip_port;
	int			port;

	ip_port = NULL;
	if (!(resp = ft_strsplit(response, ' ')))
		return (FAILURE);
	if (!resp[4] || !(ip_port = ft_strsplit(resp[4], ',')))
		return (FAILURE);
	ft_tbldel(&resp);
	port = (ip_port[4] ? ft_atoi(ip_port[4]) : 0) * 256 +
		(ip_port[5] ? ft_atoi(ip_port[5]) : 0);
	ft_tbldel(&ip_port);
	return (port ? port : (int)FAILURE);
}

int				c_passive(t_session *session)
{
	char		addr[BUFFSZ];

	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (FAILURE);
	init_session(session->psv);
	recv_msg(session->sock, addr, &session->run);
	session->psv->port = c_parse_port(addr);
	if (create_connection(session->psv,
			inet_ntoa(session->sin.sin_addr)) == FAILURE)
	{
		clean_session(session);
		printf("[-] Error establishing PASV connection\n");
		return (FAILURE);
	}
	session->mode = M_PSV;
	return (SUCCESS);
}
