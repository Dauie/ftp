#include "../incl/server.h"

int 	(*g_s_funcs[])(t_session *) = { &s_cwd, &s_help, &s_list, &s_passive,
										&s_pwd, &s_quit, &recv_file, &send_file };

char	*g_sprtd_cmds[] = { "CWD", "HELP", "LIST", "PASV", "PWD", "QUIT" , "RETR", "STOR"};

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
			g_s_funcs[i](session);
			break;
		}
	}
	if (i == len)
		send_msg(session, 1, "502 Command not implemented.\n\r");
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
		if (recv_msg(session) == EXIT_FAILURE)
			continue;
		dispatch_command(session);
	}
}