#include "../incl/server.h"

int		create_socket(t_session *session)
{
	struct protoent *proto;

	if ((proto = getprotobyname("tcp")) == 0)
		return (EXIT_FAILURE);
	if ((session->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("[-]Error creating socket\n");
		// set error code
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int 	listen_socket(t_session *session)
{
	if ((listen(session->sock, 1)) == -1)
	{
		close(session->sock);
		//set error code
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int 	bind_socket(t_session *session, char *address)
{
	session->sin.sin_family = AF_INET;
	session->sin.sin_port = htons(session->port);
	if (address)
		session->sin.sin_addr.s_addr = inet_addr(address);
	else
		session->sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(session->sock, (const struct sockaddr *)&session->sin,
			 sizeof(session->sin)) < 0)
	{
		printf("[-]Error binding socket on port %d\n", session->port);
		// set error code
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int 	options_socket(t_session *session)
{
	session->opt = TRUE;
	if (setsockopt(session->sock, SOL_SOCKET, SO_REUSEADDR,
				   (char *)&session->opt, sizeof(session->opt)) < 0)
	{
		printf("[-]Error setting options on port %d", session->port);
		// set error code
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}



