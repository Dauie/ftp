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

static char* make_zero_string(size_t len)
{
	char	*ret;

	if (!(ret = ft_strnew(len)))
		return (NULL);
	while (len--)
		ret[len] = '0';
	return(ret);
}

char		*make_length_string(off_t size)
{
	char 	*zeros;
	char 	*tmp;
	char 	*ret;

	ret = NULL;
	if (!(ret = ft_itoabse(size, 10)))
		return (NULL);
	tmp = ret;
	if (ft_strlen(ret) < 9)
	{
		if (!(zeros = make_zero_string(10 - ft_strlen(ret))))
			return (NULL);
		ret = ft_strcat(zeros, ret);
		free(tmp);
		free(zeros);
	}
	return (ret);
}

static int 		server_response(t_session *session) {
	off_t size;
	off_t off;
	char *len_str;

	off = 0;
	if ((size = lseek(session->fd, 0, SEEK_END)) == -1)
		return (EXIT_FAIL);
	len_str = make_length_string(size);
	printf("%s\n", len_str);
	send(session->cs, len_str, ft_strlen(len_str), 0);
	recv(session->cs, session->buff, 10, MSG_WAITALL);
	if (ft_strncmp(session->buff, len_str, ft_strlen(len_str)) == 0)
	{
		while ((ftp_sendfile(session->cs, session->fd, &off, size)) != -1)
			;
	}
	return(EXIT_SUCCESS);
}

static int		ftp_ls(t_session * session)
{

	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return (EXIT_FAIL);
	if ((redirect_output_fd(session->fd)) == EXIT_FAIL)
		return (EXIT_FAIL);
	execv("/bin/ls", session->argv);
	return (EXIT_SUCCESS);
}

static int		ftp_env(t_session *session)
{
	if ((redirect_output_fd(session->fd)) == EXIT_FAIL)
		return (EXIT_FAIL);
	//do the stuff.
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
		send(session->cs, "\r\r\rEOT\r\r\r", 10, 0);
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
