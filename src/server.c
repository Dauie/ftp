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
	}
	return (EXIT_SUCCESS);
}

static int		ftp_env(t_session *session)
{
	if ((redirect_output_fd(session->fd)) == EXIT_FAIL)
		return (EXIT_FAIL);
	ft_puttbl(session->env);
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
	ft_printf("command recieved: %s\n", session->buff);
	if (ft_strcmp((const char *)&session->buff, "quit" ) == 0)
	{
		printf("[+]Host has disconnected from socket %d\n", session->cs);
		close(session->cs);
		exit(EXIT_SUCCESS);
	}
	else if (ft_strncmp((const char *)&session->buff, "ls", 2) == 0)
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
		if ((ret = recv(session->cs, session->buff, BUFFSZ, MSG_WAITALL)) == -1)
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

int		create_socket(t_session *session)
{
	struct protoent *proto;

	if ((proto = getprotobyname("tcp")) == 0)
		return (EXIT_FAIL);
	if ((session->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		printf("[-]Error creating socket\n");
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

int 	listen_socket(t_session *session)
{
	if ((listen(session->sock, 1)) == -1)
	{
		close(session->sock);
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

int 	bind_socket(t_session *session)
{
	session->sin.sin_family = AF_INET;
	session->sin.sin_port = htons(session->port);
	session->sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(session->sock, (const struct sockaddr *)&session->sin, sizeof(session->sin)) < 0)
	{
		printf("[-]Error binding socket on port %d\n", session->port);
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

int 	options_socket(t_session *session)
{
	session->opt = TRUE;
	if (setsockopt(session->sock, SOL_SOCKET, SO_REUSEADDR, (char *)&session->opt, sizeof(session->opt)) < 0)
	{
		printf("[-]Error setting options on port %d", session->port);
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

int     create_server(t_session *session)
{
	create_socket(session);
	bind_socket(session);
	options_socket(session);
	listen_socket(session);
	return (EXIT_SUCCESS);
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
    session.port = ft_atoi(av[1]);
    if ((session.sock = create_socket(session.port)) == EXIT_FAIL)
	{
		printf("[-]Error creating socket for port:%i\n", session.port);
		return (EXIT_FAIL);
	}
    session.cslen = sizeof(session.csin);
    session_manager(&session);
    close(session.sock);
    return(0);
}
