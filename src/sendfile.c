#include "../incl/ftp.h"

int			prep_send(t_session *session)
{
	session->off =  0;
	session->size = 0;
	if ((session->size = lseek(session->fd, 0, SEEK_END)) == -1)
	{
		printf("[-]Error retrieving size of temp file\n");
		return(EXIT_FAIL);
	}
	add_header(session->size, session->buff);
	ft_printf("size of file from lseek %i - %s\n", session->size, session->buff);
	return (EXIT_SUCCESS);
}

int			sendfile(t_session *session)
{

}

int		send_client_msg(t_session *session, char *code, char *msg,)
{
	char *response;

	if (!(response = ft_strjoin(code, msg)))
		return (EXIT_FAIL);
	send(session->cs, response, ft_strlen(response), 0);
	return (EXIT_SUCCESS);
}

int		rand_ephem_port(void)
{
	srand((uint)time(0));
	return (rand() % EPHEM_MAX + EPHEM_MIN);
}

char 	*get_passive_addr(struct sockaddr_in *addrs, int port)
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

int 	passive(t_session *session)
{
	char *psv_addr;

	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (EXIT_FAIL);
	session->psv->port = rand_ephem_port();
	if (create_endpoint(session->psv) == EXIT_FAIL)
		return (EXIT_FAIL);
	if (!(psv_addr = get_passive_addr(&session->csin, session->psv->port)))
		return (EXIT_FAIL);
	if (send_client_msg(session, "227 Entering Passive Mode ", psv_addr) == EXIT_FAIL)
		return (EXIT_FAIL);
	free(psv_addr);
	return (EXIT_SUCCESS);
}

