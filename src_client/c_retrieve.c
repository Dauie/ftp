#include "../incl/client.h"

int 	c_retrieve(t_session *session)
{
    off_t len;

    if (session->mode == M_PSV)
    {
        session->fd = open(ft_strjoin(session->argv[1], ".txt"), O_CREAT | O_RDWR);
        if ((recv_msg(session->psv->sock, session->psv->buff, &session->psv->run)) == EXIT_FAILURE)
        {
            close_passive(session);
            recv_msg(session->sock, session->buff, &session->run);
            printf("[-]Error reading from passive socket.\n");
            return (EXIT_FAILURE);
        }
        len = ft_atoi(session->psv->buff);
        if ((recv_file(session->psv->sock, session->fd, session->psv->buff, len)) == EXIT_FAILURE)
            printf("[-]Error receiving %s from server.", session->argv[1]);
        close_passive(session);
    }
	recv_msg(session->sock, session->buff, &session->run);
	return (EXIT_SUCCESS);
}