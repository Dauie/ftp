#include "../incl/ftp.h"

void init_session(t_session *session)
{
	ft_memset(session, 0, sizeof(t_session));
	session->mode = M_NON;
	session->run = TRUE;
}

void clean_session(t_session *session)
{
	if (session->argv)
	{
		ft_tbldel(&session->argv);
		session->argv = NULL;
	}
	if (session->psv && session->mode == M_NON)
	{
		if (session->psv->argv)
			ft_tbldel(&session->psv->argv);
		free(session->psv);
		session->psv = NULL;
	}
}
