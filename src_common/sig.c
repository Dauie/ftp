#include "../incl/ftp.h"

void	kill_server(t_session *session)
{
	if (session->argv)
		ft_tbldel(&session->argv);
	if (session->env)
		ft_tbldel(&session->env);
	if (session->psv)
	{
		if (session->psv->port > 2)
			close(session->psv->port);
		if (session->psv->fd > 2)
			close(session->psv->fd);
		free(session->psv);
	}
	close(session->sock);
	free(session);
	printf("\n[+]FTP Server Terminated - (^-^)^ bye.\n");
	exit(1);
}

void	kill_client(t_session *session)
{
	send_msg(session->sock, 1, "QUIT");
	if (session->argv)
		ft_tbldel(&session->argv);
	if (session->psv)
	{
		if (session->psv->port > 2)
			close(session->psv->port);
		if (session->psv->fd > 2)
			close(session->psv->fd);
		free(session->psv);
	}
	close(session->sock);
	free(session);
	printf("\n[+](^-^)^ bye.\n");
	exit(1);
}

void	handel_sig(int sig)
{
	if (sig == SIGINT)
	{
		g_session->kill = TRUE;
	}
}
