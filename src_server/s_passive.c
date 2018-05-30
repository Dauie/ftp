#include "../incl/server.h"

static int		rand_ephem_port(void)
{
	srand((uint)time(0));
	return (rand() % EPHEM_MAX + EPHEM_MIN);
}

static char 	*make_passive_addr(struct sockaddr_in *addrs, int port)
{
	int		i;
	char 	**ip_port;
	char 	*ret;

	i = -1;
	if (!(ip_port = ft_memalloc(sizeof(char *) * 6)))
		return (NULL);
	if (!(ret = ft_memalloc(sizeof(char) * 18)))
		return (NULL);
	while (++i < 4)
		ip_port[i] = ft_itoa(addrs->sin_addr.s_addr >> (i * 8) & 255);
	while (i++ < 6)
		ip_port[i] = ft_itoa(port >> ((i - 5) * 8) & 255);
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

int 	s_passive(t_session *session)
{
	char *psv_addr;

	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (EXIT_FAILURE);
	session->psv->port = rand_ephem_port();
	if (create_endpoint(session->psv, NULL) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!(psv_addr = make_passive_addr(&session->csin, session->psv->port)))
		return (EXIT_FAILURE);
	if (send_msg(session, 3, "227 Entering Passive Mode. ", psv_addr, "\n\r") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free(psv_addr);
	return (EXIT_SUCCESS);
}