#include "../incl/ftp.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(EXIT_FAIL);
}

static int		redirect_output_fd(int fd)
{
    if (dup2(fd, STDERR_FILENO) < 0)
    {
        printf("[-]Error redirecting STDERR to file descriptor %d\n", fd);
        return (EXIT_FAIL);
    }
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        printf("[-]Error redirecting STDOUT to file descriptor %d\n", fd);
        return (EXIT_FAIL);
    }
    return (EXIT_SUCCESS);
}





static int 		server_response(t_session *session)
{
	char 		len_str[10];
	int 		ret;

	ret = 1;
	// Get file size
	if ((prep_send(session)) ==  EXIT_FAIL)
		return (EXIT_FAIL);
	// Prepare string representing full file size
	if ((add_header(session->size, session->buff)) == EXIT_FAIL)
		return (EXIT_FAIL);
	// Send client the size of the file
	send(session->cs, session->buff, 10, 0);
	ft_strncpy(len_str, session->buff, 10);
	ft_bzero(session->buff , BUFFSZ);
	// The client will reply with file length when prepared to receive.
	recv(session->cs, session->buff, 10, MSG_WAITALL);
	// Verify the client has sufficient has space for file.
	if (ft_strncmp(session->buff, len_str, 10) == 0)
	{
		// send response/file
		while (ret)
			ret = ftp_sendfile(session->cs, session->fd, &session->off, session->size);
	}
	return(EXIT_SUCCESS);
}

static int		ftp_ls(t_session * session)
{
	pid_t			pid;
	struct rusage 	rusage;
	int 			wait_status;

	pid = 0;
	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return (EXIT_FAIL);
	if ((redirect_output_fd(session->fd)) == EXIT_FAIL)
		return (EXIT_FAIL);
	if ((pid = fork()) == -1)
	{
		printf("[-]Error forking process to execute ls\n");
		return (EXIT_FAIL);
	}
	else if (pid > 0)
		execv("/bin/ls", session->argv);
	else if (pid == 0)
	{
		if (wait4(pid, &wait_status, 0, &rusage ) == -1)
		{
			printf("[-]Error waiting on child process to complete\n");
			return(EXIT_FAIL);
		}
		server_response(session);
	}
	return (EXIT_SUCCESS);
}

static int		ftp_env(t_session *session)
{
	if ((redirect_output_fd(session->fd)) == EXIT_FAIL)
		return (EXIT_FAIL);
	ft_puttbl(session->env);
	server_response(session);
	return (EXIT_SUCCESS);
}

static int			execute_client_cmd(t_session *session, int(fn(t_session *)))
{
	pid_t			pid;
	struct rusage 	rusage;
	int 			wait_status;

	if ((create_temp_file(session)) == EXIT_FAIL)
		return (EXIT_FAIL);
	if ((pid = fork()) == -1)
	{
		printf("[-]Error forking process\n");
		return (EXIT_FAIL);
	}
	else if (pid == 0)
		fn(session);
	else if (pid > 0)
	{
		if (wait4(pid, &wait_status, 0, &rusage ) == -1)
		{
			printf("[-]Error waiting on child process to complete\n");
			return(EXIT_FAIL);
		}
		if (session->argv)
			ft_tbldel(session->argv, ft_tbllen(session->argv));
	}
	return (EXIT_SUCCESS);
}

static void     act_accordingly(t_session *session)
{
	if (ft_strncmp((const char *)&session->buff, "ls", 2) == 0)
		execute_client_cmd(session, ftp_ls);
	else if (ft_strncmp((const char *)&session->buff, "env", 3) == 0)
		execute_client_cmd(session, ftp_env);
}

static void     manage_client_session(t_session *session)
{
    ssize_t     ret;

    ret = 0;
    while (TRUE)
    {
		ft_bzero(session->buff, BUFFSZ);
		if ((ret = recv(session->cs, session->buff, BUFFSZ, 0)) == -1)
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
    while (TRUE)
    {
        if (!(session->cs = accept(session->sock, (struct sockaddr*)&session->csin, &session->cslen)))
        {
            printf("[-]Error accepting connection.");
                break;
        }
        printf("[+]New connection accepted from %s:%d on sd %d\n",
               inet_ntoa(session->csin.sin_addr), ntohs(session->csin.sin_port), session->cs);
        if ((session->pid = fork()) == -1)
        {
            close(session->cs);
            printf("[-]Error forking process for new connection\n");
            continue;
        }
        else if (session->pid > 0)
        {
			close(session->cs);
			signal(SIGCHLD,SIG_IGN);
        }
        else if (session->pid == 0)
        {
            close(session->sock);
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
    if ((listen(sock, 4)) == 0)
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
