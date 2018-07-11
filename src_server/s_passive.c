#include "../incl/server.h"

static int		rand_ephem_port(void)
{
	srand((uint)time(0));
	return ((rand() % ((EPHEM_MAX - EPHEM_MIN) + 1)) + EPHEM_MIN);
}

static char 	*make_passive_addr(unsigned int addr, unsigned short port)
{
	int		i;
	char 	**ip_port;
	char 	*ret;

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
	ft_tbldel(ip_port, 6);
	return (ret);
}

int			s_passive(t_session *session) {
	char	*psv_addr;
	struct	addrinfo;

	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (FAILURE);
	init_session(session->psv);
	session->psv->port = rand_ephem_port();
	if (create_endpoint(session->psv, NULL) == FAILURE)
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing. \r\n");
		return (FAILURE);
	}
	if (!(psv_addr = make_passive_addr(session->psv->sin.sin_addr.s_addr, session->psv->sin.sin_port)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing. \r\n");
		return (FAILURE);
	}
	send_msg(session->cs, 3, "227 Entering Passive Mode. ", psv_addr, " \r\n");
	if (accept_connection(session->psv) == FAILURE)
		return (FAILURE);
	session->mode = M_PSV;
	free(psv_addr);
	return (SUCCESS);
}
