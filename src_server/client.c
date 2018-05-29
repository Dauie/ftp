#include "../incl/server.h"

int 	(*g_svrfuncs[])(t_session *) = { &s_cwd, &s_help, &s_list, &s_passive,
										&s_pwd, &s_quit, &recv_file, &send_file };

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
			g_svrfuncs[i](session);
			break;
		}
	}
	if (i == len)
		send_client_msg(session, 1, "502 Command not implemented.\n\r");
	if (session->argv)
		ft_tbldel(session->argv, ft_tbllen(session->argv));
}

void     manage_client_session(t_session *session)
{
	ssize_t     ret;

	ret = 0;
	while (session->run)
	{
		ft_bzero(session->buff, BUFFSZ);
		if ((ret = recv(session->cs, session->buff, BUFFSZ, 0)) == -1)
		{
			printf("[-]Error reading from socket\n");
			continue;
		}
		else if (ret == 0)
		{
			printf("[-]Client disconnected from session on sd %d.\n", session->cs);
			close(session->cs);
			session->run = FALSE;
		}
		else
			dispatch_command(session);
	}
}