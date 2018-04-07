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
    if (dup2(session->cs, STDOUT_FILENO) < 0)
    {
        printf("[-]Error redirecting STDOUT to socket descriptor %d\n", session->cs);
        return (EXIT_FAIL);
    }
    return (EXIT_SUCCESS);
}

static int		ftp_ls(t_session * session)
{

	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return (EXIT_FAIL);
	if ((redirect_output_tosock(session)) == EXIT_FAIL)
		return (EXIT_FAIL);
	execv("/bin/ls", session->argv);
	return (EXIT_SUCCESS);
}

static int		ftp_env(t_session *session)
{
	if ((redirect_output_tosock(session)) == EXIT_FAIL)
		return (EXIT_FAIL);
	ft_puttbl(session->env);
	return (EXIT_SUCCESS);
}

static int			execute_client_cmd(t_session *session, int(fn(t_session *)))
{
	pid_t			pid;
	struct rusage 	rusage;
	int 			wait_status;

	if ((pid = fork()) == -1)
	{
		printf("[-]Error forking process\n");
		send(session->cs, "\r\r\rEOT\r\r\r", 10, 0);
		return (EXIT_FAIL);
	}
	else if (pid == 0)
		fn(session);
	else if (pid > 0)
	{
		if (wait4(pid, &wait_status, 0, &rusage ) == -1)
		{
			printf("[-]Error waiting on child process to complete");
			return(EXIT_FAIL);
		}
		sleep(1);
		if (session->argv)
			ft_tbldel(session->argv, ft_tbllen(session->argv));
		send(session->cs, "\r\r\rEOT\r\r\r", 10, 0);
	}
	return (EXIT_SUCCESS);
}

//int             ftp_cd(t_session *session, int i)
//{
//
//}

static void     act_accordingly(t_session *session)
{
    int ret;

    ret = 0;
	if (ft_strncmp((const char *)&session->buff, "ls", 2) == 0)
		ret = execute_client_cmd(session, ftp_ls);
	else if (ft_strncmp((const char *)&session->buff, "env", 3) == 0)
		ret = execute_client_cmd(session, ftp_env);
    //other things like cd... etc..
    if (ret == EXIT_FAIL)
        send(session->cs, "\r\r\rEOT\r\r\r", 10, 0);
}

static void     manage_client_session(t_session *session)
{
    ssize_t     ret;

    ret = 0;
    while (TRUE)
    {
		ft_bzero(session->buff, BUFFSZ);
		if ((ret = recv(session->cs, session->buff, 1023, 0)) == -1)
        {
            printf("\n[-]Error reading from socket\n");
            continue;
        }
		else if (ret == 0)
		{
			printf("[-]Server disconnected from session on sd %d.\n", session->cs);
			close(session->cs);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp((const char *)&session->buff, "quit" ) == 0)
		{
			printf("[+]Host has disconnected from socket %d\n", session->cs);
			close(session->cs);
			exit(EXIT_SUCCESS);
		}
		else
			act_accordingly(session);
    }
}

static void    session_manager(t_session *session)
{
	pid_t			pid;

    while (TRUE)
    {
        if (!(session->cs = accept(session->sock, (struct sockaddr*)&session->csin, &session->cslen)))
        {
            printf("[-]Error accepting connection.");
                break;
        }
        printf("[+]New connection accepted from %s:%d on sd %d\n",
               inet_ntoa(session->csin.sin_addr), ntohs(session->csin.sin_port), session->cs);
        if ((pid = fork()) == -1)
        {
            close(session->cs);
            printf("[-]Error forking process for new connection\n");
            continue;
        }
        else if (pid > 0)
        {
			close(session->cs);
			signal(SIGCHLD,SIG_IGN);
        }
        else if (pid == 0)
        {
            close(session->sock);
            manage_client_session(session);
        }
    }
}

static void init_session(t_session * session)
{
	session->port = 0;
	session->sock = 0;
	session->cs = 0;
	session->pid = 0;
	session->cslen = 0;
	session->env = NULL;
	session->argv = NULL;
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
	extern char **environ;

    if (ac != 2)
        usage(av[0]);
	init_session(&session);
	if (!(session.env = ft_tbldup(environ, ft_tbllen(environ))))
		return (EXIT_FAIL);
    session.port = atoi(av[1]);
    session.sock = create_server(session.port);
    session.cslen = sizeof(session.csin);
    session_manager(&session);
    close(session.sock);
    return(0);
}
