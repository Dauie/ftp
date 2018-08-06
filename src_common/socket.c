#include "../incl/ftp.h"

int		create_socket(t_session *session, char *address)
{
	struct protoent *proto;

	if ((proto = getprotobyname("tcp")) == 0)
		return (FAILURE);
	if ((session->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("[-]Error creating socket\n");
		return (FAILURE);
	}
	session->sin.sin_family = AF_INET;
	session->sin.sin_port = htons(session->port);
	if (address)
		session->sin.sin_addr.s_addr = inet_addr(address);
	else
		session->sin.sin_addr.s_addr = htonl(INADDR_ANY);
	session->cslen = sizeof(session->csin);
	return (SUCCESS);
}

int 	listen_socket(t_session *session)
{

	if ((listen(session->sock, 1)) == -1)
	{
		close(session->sock);
		return (FAILURE);
	}
	return (SUCCESS);
}

int 	bind_socket(t_session *session)
{
	if (bind(session->sock, (const struct sockaddr *)&session->sin,
				sizeof(session->sin)) < 0)
	{
		printf("[-]Error binding socket on port %d\n", session->port);
		// set error code
		return (FAILURE);
	}
	return (SUCCESS);
}

int 	options_socket(t_session *session)
{
	int opt;
	opt = TRUE;
	if (setsockopt(session->sock, SOL_SOCKET, SO_REUSEADDR,
				(char *)&opt, sizeof(opt)) < 0)
	{
		printf("[-]Error setting options on port %d", session->port);
		return (FAILURE);
	}
	return (SUCCESS);
}



