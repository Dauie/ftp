#include "../incl/ftp.h"

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
		return (EXIT_FAIL);
	tmp = ret;
	if (ft_strlen(ret) < 10)
	{
		if (!(zeros = make_zero_string(10 - ft_strlen(ret))))
			return (EXIT_FAIL);
		if (!(ret = ft_strconcat(zeros, ret)))
			return (EXIT_FAIL);
		free(tmp);
		free(zeros);
	}
	ft_strcpy(buff, ret);
	free(ret);
	return (EXIT_SUCCESS);
}

void init_session(t_session * session)
{
	session->port = 0;
	session->sock = 0;
	session->cs = 0;
	session->pid = 0;
	session->off = 0;
	session->size = 0;
	session->cslen = 0;
	session->env = NULL;
	session->argv = NULL;
}
