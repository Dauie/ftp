#include "../incl/ftp.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(EXIT_FAIL);
}

static int		redirect_output_tosock(t_session *session)
{
    if (dup2(session->cs, STDERR_FILENO) < 0)
    {
        printf("[-]Error redirecting STDERR to socket descriptor %d\n", session->cs);
        return (EXIT_FAIL);
    }
//	if (dup2(session->csockets[i], STDIN_FILENO) < 0)
//	{
//		printf("[-]Error redirecting STDIN to socket descriptor %d\n", session->cs);
//		return (EXIT_FAIL);
//	}
    if (dup2(session->cs, STDOUT_FILENO) < 0)
    {
        printf("[-]Error redirecting STDOUT to socket descriptor %d\n", session->cs);
        return (EXIT_FAIL);
    }
    return (EXIT_SUCCESS);
}

static int		ftp_ls(t_session * session)
{

    char			**argv;

    argv = NULL;

    if (redirect_output_tosock(session) == EXIT_FAIL)
    {
        ft_strcpy(session->buff, "[-]A server error has occurred when redirecting output for communication");
        return (EXIT_FAIL);
    }
    if (!(argv = ft_strsplit(session->buff, ' ')))
        return (EXIT_FAIL);
    execv("/bin/ls", &argv[1]);
    ft_tbldel(argv, ft_tbllen(argv));
    return (EXIT_SUCCESS);
}

//int             ftp_cd(t_session *session, int i)
//{
//
//}

static void     act_accordingly(t_session *session)
{

    if (ft_strcmp((const char *)&session->buff, "quit" ) == 0)
    {
        printf("[+]Host has disconnected from socket %d\n", session->cs);
        close(session->cs);
        exit(EXIT_SUCCESS);
    }
    else if (ft_strncmp((const char *)&session->buff, "ls", 2) == 0)
        ftp_ls(session);
//    else if (ft_strncmp((const char *)&session->buff, "cd", 2) == 0)
//        ftp_cd(session);
    send(session->cs, session->buff, strlen(session->buff), 0);
}

static void     manage_client_session(t_session *session)
{
    while (TRUE)
    {
        ft_bzero(session->buff, BUFFSZ);
        if (!(read(session->cs, session->buff, 1023)))
        {
            printf("[-]Error reading from socket\n");
            continue;
        }
        act_accordingly(session);
    }
}

static void    session_manager(t_session *session)
{
    int ret;

    ret = 0;
    while (TRUE)
    {
        if (!(session->cs = accept(session->sock, (struct sockaddr*)&session->csin, &session->cslen)))
        {
            printf("[-]Error accepting connection.");
                break;
        }
        printf("[+]New connection accepted from %s:%d on sd %d\n",
               inet_ntoa(session->csin.sin_addr), ntohs(session->csin.sin_port), session->cs);
        if ((ret = fork()) == -1)
        {
            printf("[-]Error forking process for new connection\n");
            continue;
        }
        if (ret == 0)
        {
            session->pid = getpid();
            printf("%d forked()", session->pid);
            manage_client_session(session);
        }
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
    session_manager(&session);
    close(session.cs);
    close(session.sock);
    return(0);
}
