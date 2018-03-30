#include <errno.h>
#include "../incl/ftp.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(-1);
}

int     create_server(int port)
{
    int sock;
	int opt = TRUE;
    struct protoent *proto;
    struct sockaddr_in  sin;

    sock = 0;
    proto = getprotobyname("tcp");
    if (proto == 0)
        return (-1);
    if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("[-]Error creating socket\n");
		exit(-1);
	}
    sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		printf("[-]Error setting options on master socket %d", sock);
	}
    if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        printf("[-]Error binding socket %d\n", sock);
        exit(-1);
    }
    if (listen(sock, 4) == 0)
        printf("[+]Awaiting request...\n");
    else
        printf("[-]Error in binding port %d may be in use\n", CMD_PORT);
    return (sock);
}

/*
 * Initialize all client sockets to 0
 * */
void	init_csockets(int *array, size_t length)
{
	while (length-- > 0)
		array[length] = 0;
}

void	add_child_sockets(int *max_sd, int len, int *sockets, fd_set *readfds)
{
	int i;

	i = -1;
	while (++i < len)
	{
		if (sockets[i] > 0)
			FD_SET(sockets[i], readfds);
		if (sockets[i] > *max_sd)
			*max_sd = sockets[i];
	}
}

void	add_active_sd(int *sockets, int sd, int length)
{
	int i;

	i = -1;
	while (++i < length)
	{
		if (sockets[i] == 0)
		{
			sockets[i] = sd;
			break;
		}
	}
}


static void     close_client_connection(t_session *session, int i)
{
    printf("[+]Host has disconnected from socket %d\n", session->csockets[i]);
    close(session->csockets[i]);
    session->csockets[i] = 0;
}

static void     act_accordingly(t_session *session, int i)
{

    if (ft_strcmp((const char *)&session->buff, "quit" ) == 0)
        close_client_connection(session, i);
    else if (ft_strncmp((const char *)&session->buff, "ls", 2) == 0)
    {
       ;
    }
    else
    {
        send(session->csockets[i], session->buff, strlen(session->buff), 0);
        ft_bzero(&session->buff, 1024);
    }
}

static void    manage_incoming_data(t_session *session)
{
    int i = -1;
    while (++i < MAX_CLIENTS)
    {
        if (FD_ISSET(session->csockets[i], &session->readfds))
        {
            if (!(read(session->csockets[i], session->buff, 1023)))
                continue;
            act_accordingly(session, i);
        }
    }
}

static void    run_session(t_session *session)
{
    while (TRUE)
    {
        ft_bzero(&session->buff, 1024);
        FD_ZERO(&session->readfds);
        FD_SET(session->sock, &session->readfds);
        session->max_sd = session->sock;
        add_child_sockets(&session->max_sd, MAX_CLIENTS, (int*)&session->csockets, &session->readfds);
        if ((select( session->max_sd + 1 , &session->readfds , NULL , NULL , NULL)) < 0 && (h_errno != EINTR))
            printf("[-]Error with select()\n");
        if (FD_ISSET(session->sock, &session->readfds))
        {
            if (!(session->cs = accept(session->sock, (struct sockaddr*)&session->csin, &session->cslen)))
            {
                printf("[-]Error accepting connection.");
                break;
            }
            add_active_sd((int*)&session->csockets, session->cs, MAX_CLIENTS);
            printf("[+]New connection accepted from %s:%d on sd %d\n",
                   inet_ntoa(session->csin.sin_addr), ntohs(session->csin.sin_port), session->cs);
        }
        else
            manage_incoming_data(session);
    }
}

int main(int ac, char **av)
{
    t_session session;

    if (ac != 2)
        usage(av[0]);
    session.port = atoi(av[1]);
    session.sock = create_server(session.port);
    session.cslen = sizeof(session.csin);
    init_csockets((int*)&session.csockets, MAX_CLIENTS);
    run_session(&session);
	close(session.cs);
    close(session.sock);
    return(0);
}
