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


static void     dispatch_command(t_session *session)
{
	int	i;
	int len;

	i = -1;
	len = (int)ft_tbllen(g_sprtd_cmds);
	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return ;
	while (++i < len)
	{
		if (ft_strncmp(session->buff,
			   g_sprtd_cmds[i], ft_strlen(g_sprtd_cmds[i])) == 0)
		{
			g_cmds[i](session);
			break;
		}
	}
	if (i == len)
		send_client_msg(session, "502", "Command not implemented");
	ft_tbldel(session->argv, ft_tbllen(session->argv));
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
			dispatch_command(session);
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
