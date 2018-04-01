#include "../incl/ftp.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(EXIT_FAIL);
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

int     create_server(int port)
{
    int sock;
    int opt = TRUE;
    struct protoent *proto;
    struct sockaddr_in  sin;

    sock = 0;
    proto = getprotobyname("tcp");
    if (proto == 0)
        return (EXIT_FAIL);
    if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
    {
        printf("[-]Error creating socket\n");
        exit(EXIT_FAIL);
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
        exit(EXIT_FAIL);
    }
    if (listen(sock, 4) == 0)
        printf("[+]Awaiting request...\n");
    else
        printf("[-]Error in binding port %d may be in use\n", CMD_PORT);
    return (sock);
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
    run_sessions8(&session);
    close(session.cs);
    close(session.sock);
    return(0);
}
