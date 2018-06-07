#include "../incl/server.h"

void    usage(char *str)
{
    printf("Useage: %s <port>\n", str);
    exit(EXIT_FAILURE);
}

int 	s_help(t_session *session)
{
	if (send_msg(session->cs, 9, "FTP Server Usage:\n",
				 "CWD <dir> - change working directory.\n",
				 "HELP - Lists all supported commands\n",
				 "LIST <path> - List files/directories in path\n",
				 "PASV - Enter passive mode\n",
				 "PWD - Prints working directory\n",
				 "QUIT - Closes connection and quits program.\n",
				 "RETR <path> - Retrieve file at path.\n",
				 "STOR <path> - Store file at path.\n") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void    session_manager(t_session *session)
{
    while (session->run)
    {
		if (accept_connection(session) == EXIT_FAILURE)
			continue;

        if ((session->pid = fork()) == -1)
        {
            close(session->cs);
            continue;
        }
        else if (session->pid > 0)
        {
			close(session->cs);
//			signal(SIGINT, handel_sig);
			signal(SIGCHLD, handel_sig);
        }
        else if (session->pid == 0)
        {
            close(session->sock);
            manage_client_session(session);
        }
    }
}

int		accept_connection(t_session *session)
{
	if (!(session->cs = accept(session->sock,
			 (struct sockaddr*)&session->csin, &session->cslen)))
	{
		//Set error code
		return (EXIT_FAILURE);
	}
	printf("[+]New connection %s:%d on sd %d\n",
		   inet_ntoa(session->csin.sin_addr), ntohs(session->csin.sin_port), session->cs);;
	return (EXIT_SUCCESS);
}

int     create_endpoint(t_session *session, char *address)
{
	if (create_socket(session, address) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (bind_socket(session) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (options_socket(session) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (listen_socket(session) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
    t_session session;
	extern char **environ;

    if (ac != 2)
        usage(av[0]);
	init_session(&session);
	if (!(session.env = ft_tbldup(environ, ft_tbllen(environ))))
		return (EXIT_FAILURE);
    session.port = ft_atoi(av[1]);
    if (create_endpoint(&session, NULL) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printf("[+]Server started on port %d\n", session.port);
	session.cslen = sizeof(session.csin);
    session_manager(&session);
    close(session.sock);
    return(0);
}
