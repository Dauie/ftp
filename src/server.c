#include "../incl/ftp.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(EXIT_FAIL);
}

void	quit(t_session *session)
{
	if (ft_strcmp((const char *)&session->buff, "quit" ) == 0)
	{
		printf("[+]Host has disconnected from socket %d\n", session->cs);
		close(session->cs);
		exit(EXIT_SUCCESS);
	}
}


static int		env(t_session *session)
{
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

	else if (ft_strncmp((const char *)&session->buff, "ls", 2) == 0)
		execute_client_cmd(session, list);
	else if (ft_strncmp((const char *)&session->buff, "env", 3) == 0)
		execute_client_cmd(session, env);
}

static void     manage_client_session(t_session *session)
{
    ssize_t     ret;

    ret = 0;
    while (session->run)
    {
		ft_bzero(session->buff, BUFFSZ);
		if ((ret = recv(session->cs, session->buff, BUFFSZ, 0)) == -1)
        {
            printf("\n[-]Error reading from socket\n");
            continue;
        }
		else if (ret == 0)
		{
			printf("[-]Client disconnected from session on sd %d.\n", session->cs);
			close(session->cs);
			session->run = 0;
		}
		else
			act_accordingly(session);
    }
}

static void    session_manager(t_session *session)
{
    while (session->run)
    {
		if (accept_connection(session) == EXIT_FAIL)
			continue;
        printf("[+]New connection %s:%d on sd %d\n",
               inet_ntoa(session->csin.sin_addr), ntohs(session->csin.sin_port), session->cs);
        if ((session->pid = fork()) == -1)
        {
            close(session->cs);
            continue;
        }
        else if (session->pid > 0)
        {
			close(session->cs);
			signal(SIGINT, handel_sig);
			signal(SIGCHLD, handel_sig);
        }
        else if (session->pid == 0)
        {
            close(session->sock);
            manage_client_session(session);
        }
    }
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
    if (create_endpoint(&session) == EXIT_FAIL)
		return (EXIT_FAIL);
	session.cslen = sizeof(session.csin);
    session_manager(&session);
    close(session.sock);
    return(0);
}
