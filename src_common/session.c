#include "../incl/ftp.h"

void init_session(t_session *session)
{
	session->argv = NULL;
	session->cs = 0;
	session->cslen = 0;
	session->env = NULL;
	session->mode = M_NON;
	session->pid = 0;
	session->port = 0;
	session->run = TRUE;
	session->sock = 0;
}

void close_session(t_session *session)
{
	if (session->argv)
	{
		ft_tbldel(session->argv, ft_tbllen(session->argv));
		session->argv = NULL;
	}
	if (session->psv)
	{
		free(session->psv);
		session->psv = NULL;
	}
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

int			add_header(off_t size, char *buff)
{
	char 	*zeros;
	char 	*tmp;
	char 	*ret;

	if (!(ret = ft_itoabse(size, 10)))
		return (EXIT_FAILURE);
	tmp = ret;
	if (ft_strlen(ret) < 10)
	{
		if (!(zeros = make_zero_string(10 - ft_strlen(ret))))
			return (EXIT_FAILURE);
		if (!(ret = ft_strconcat(zeros, ret)))
			return (EXIT_FAILURE);
		free(tmp);
		free(zeros);
	}
	ft_strcpy(buff, ret);
	free(ret);
	return (EXIT_SUCCESS);
}