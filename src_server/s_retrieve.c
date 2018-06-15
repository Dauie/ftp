#include "../incl/server.h"

static int			is_file(char *pwd, char *file)
{
    struct stat	    sd;
    char            *buff;

    if (!(buff = ft_strjoinn(3, pwd, "/", file)))
        return (EXIT_FAILURE);
    if (stat(buff, &sd) == -1 || (S_ISREG(sd.st_mode) == 0))
    {
        free(buff);
        return (EXIT_FAILURE);
    }
    free(buff);
    return (EXIT_SUCCESS);
}

static off_t     prepare_send(t_session *session)
{
    char    dir[256];
    char    *str;

    if (!(str = getcwd(dir, 255)))
    {
        close_passive(session, T_SVR);
        send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing. \r\n");
        return (EXIT_FAILURE);
    }
    if (is_file(str, session->argv[1]) == EXIT_FAILURE)
    {
        close_passive(session, T_SVR);
        send_msg(session->cs, 1, "550 Requested action not taken. Cannot transfer directory. \r\n");
        return (EXIT_FAILURE);
    }
    if (!session->argv[1] || (session->fd = open(session->argv[1], O_RDONLY)) < 0)
    {
        close_passive(session, T_SVR);
        send_msg(session->cs, 1, "550 Requested action not taken. File unavailable. \r\n");
        return (EXIT_FAILURE);
    }
    send_msg(session->psv->cs, 1, (str = ft_itoa(get_file_size(session->argv[1]))));
    free(str);
    return (get_file_size(session->argv[1]));
}


int 	s_retrieve(t_session *session)
{
    off_t len;

	if (session->mode == M_PSV)
	{

        if ((len = prepare_send(session)) == EXIT_FAILURE)
            return (EXIT_FAILURE);
		if (send_file(session->psv->cs, session->fd, session->psv->buff, len) ==  EXIT_FAILURE)
		{
			close_passive(session, T_SVR);
            close(session->fd);
			send_msg(session->cs, 1, "451 Requested action not taken. Error in file transfer. \r\n");
			return (EXIT_FAILURE);
		}
		send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful. \r\n");
		close_passive(session, T_SVR);
        close(session->fd);
		return (EXIT_SUCCESS);
	}
	send_msg(session->cs, 1, "425 Can't open data connection. Use PASV. \r\n");
	return (EXIT_FAILURE);
}

