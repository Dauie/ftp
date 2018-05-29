#include "../incl/server.h"

int			send_file(t_session *session)
{
	send_client_msg(session->psv, 1, "502 [-]Command not implemented.\n\r");
	send_client_msg(session->psv, 1, "226 [+]Closing data connection.\n\r");
	return (EXIT_SUCCESS);
}

int		send_msg(t_session *session, int n, ...)
{
	va_list	ap;
	char	**tmp;
	char	*res;
	int		len;
	int		i;

	len = 0;
	i = -1;
	va_start(ap, n);
	if (!(tmp = ft_memalloc(sizeof(char *) * (n + 1))))
		return (NULL);
	while (++i < n)
	{
		tmp[i] = va_arg(ap, char *);
		len += ft_strlen(tmp[i]);
	}
	if (!(res = ft_memalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = -1;
	while (++i < n)
		ft_strcat(res, tmp[i]);
	free(tmp);
	send(session->cs, res, len, 0);
	return (EXIT_SUCCESS);
}

