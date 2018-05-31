#include "../incl/server.h"

int 	(*g_s_funcs[])(t_session *) = { &s_cwd, &s_help, &s_list, &s_passive,
										&s_pwd, &s_quit, &s_retrieve, &s_store };

char	*g_sprtd_cmds[] = { "CWD", "HELP", "LIST", "PASV", "PWD", "QUIT" , "RETR", "STOR"};

static void     dispatch_command(t_session *session)
{
	int	i;
	int len;

	ft_printf("in dispatch\n");
	i = -1;
	len = (int)ft_tbllen(g_sprtd_cmds);
	if (!(session->argv = ft_strsplit(session->buff, ' ')) || !session->argv[0])
		return;
	ft_printf("%p\n", (void*)session->argv[0]);
	while (session->argv[0] && ++i < len)
	{
		write(1, "here", 4);
		if (ft_strncmp(session->argv[0],
					   g_sprtd_cmds[i], ft_strlen(g_sprtd_cmds[i])) == 0)
		{
			g_s_funcs[i](session);
			break;
		}
	}
	if (i == len )
		send_msg(session->cs, 1, "502 Command not implemented.\n\r");
	if (session->argv)
		ft_tbldel(session->argv, ft_tbllen(session->argv));
}

void     manage_client_session(t_session *session)
{
	ssize_t     ret;
	int 		i = 0;
	ret = 0;
	while (session->run)
	{
		ft_bzero(session->buff, BUFFSZ);
		if (recv_msg(session->cs, session->buff, &session->run) == EXIT_FAILURE) {
			write(1, "failed\n", 7);
			continue;
		}
		printf("i = %d\n", i++);
		dispatch_command(session);
	}
}