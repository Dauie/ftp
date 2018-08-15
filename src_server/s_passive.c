/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_passive.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 17:16:26 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/14 17:48:34 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

static int		rand_ephem_port(void)
{
	srand((uint)time(0));
	return ((rand() % ((EPHEM_MAX - EPHEM_MIN) + 1)) + EPHEM_MIN);
}

static char		*make_passive_addr(unsigned int addr, unsigned short port)
{
	int			i;
	char		**ip_port;
	char		*ret;

	i = -1;
	if (!(ip_port = ft_memalloc(sizeof(char *) * 7)))
		return (NULL);
	if (!(ret = ft_memalloc(sizeof(char) * (4 * 6))))
		return (NULL);
	while (++i < 4)
		ip_port[i] = ft_itoa(addr >> (i * 8) & 255);
	i--;
	while (++i < 6)
		ip_port[i] = ft_itoa((port >> ((i - 4) * 8) & 255));
	i = -1;
	while (++i < 6)
	{
		ft_strcat(ret, ip_port[i]);
		if (i < 5)
			ft_strcat(ret, ",");
	}
	ft_tbldel(&ip_port);
	return (ret);
}

static int		make_passive_connection(t_session *session)
{
	char		*psv_addr;

	if (create_endpoint(session->psv, NULL) == FAILURE)
	{
		send_msg(session->cs, 1, "451 Requested action aborted."
				" Local error in processing. \r\n");
		return (FAILURE);
	}
	ft_gethstaddr(session->buff);
	inet_pton(AF_INET, session->buff, &session->psv->sin.sin_addr.s_addr);
	if (!(psv_addr = make_passive_addr(session->psv->sin.sin_addr.s_addr,
									session->psv->sin.sin_port)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted."
				" Local error in processing. \r\n");
		return (FAILURE);
	}
	send_msg(session->cs, 3, "227 Entering Passive Mode. ", psv_addr, " \r\n");
	free(psv_addr);
	return (SUCCESS);
}

int				s_passive(t_session *session)
{
	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (FAILURE);
	init_session(session->psv);
	session->psv->port = rand_ephem_port();
	if (make_passive_connection(session) == FAILURE)
		return (FAILURE);
	if (accept_connection(session->psv) == FAILURE)
	{
		close_passive(session, T_SVR);
		return (FAILURE);
	}
	session->mode = M_PSV;
	return (SUCCESS);
}
