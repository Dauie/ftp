#include "../incl/client.h"

static int 	c_parse_port(char *response)
{
	char **resp;
	char **ip_port;
	int port;

	resp = NULL;
	ip_port = NULL;
	if (!(resp = ft_strsplit(response, ' ')))
		return (EXIT_FAILURE);
	if (!resp[4] || !(ip_port = ft_strsplit(resp[4], ',')))
		return (EXIT_FAILURE);
	ft_tbldel(resp, ft_tbllen(resp));
	port = (ip_port[4] ? ft_atoi(ip_port[4]) : 0) * 256 +
			(ip_port[5] ? ft_atoi(ip_port[5]) : 0);
	return (port ? port : EXIT_FAILURE);
}

int 	c_passive(t_session *session)
{
	char	addr[BUFFSZ];

	if (!(session->psv = ft_memalloc(sizeof(t_session))))
		return (EXIT_FAILURE);
	init_session(session->psv);
	printf("[+]Initiating PASV mode\n");
	recv_msg(session->sock, addr, &session->run);
    printf("%s", addr);
	session->psv->port = c_parse_port(addr);
	if (create_connection(session->psv, inet_ntoa(session->sin.sin_addr)) == EXIT_FAILURE)
	{
		clean_session(session);
		printf("[-] Error establishing PASV connection\n");
		return (EXIT_FAILURE);
	}
	session->mode = M_PSV;
	return (EXIT_SUCCESS);
}
