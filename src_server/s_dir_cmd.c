#include "../incl/server.h"


int		redirect_output_fd(int fd)
{
	if (dup2(fd, STDERR_FILENO) < 0)
	{
		printf("[-]Error redirecting STDERR to socket: %d\n", fd);
		return (EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		printf("[-]Error redirecting STDOUT to socket: %d\n", fd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		s_cwd(t_session *session)
{
	if (chdir(session->buff) == -1)
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing.\n\r");
		return (EXIT_FAILURE);
	}
	send_msg(session->cs, 1, "200 Working directory changed\n\r");
	return (EXIT_SUCCESS);
}

int		s_pwd(t_session *session)
{
	char	dir[256];
	char 	*res;

	if ((res = getcwd(dir, 255)) == NULL)
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing.\n\r");
		return (EXIT_FAILURE);
	}
	send_msg(session->cs, 3, "200 ", res, "\n\r");
	recv_msg(session->cs, session->buff, &session->run);
	return (EXIT_SUCCESS);
}

int 		s_store(t_session *session)
{
	send_msg(session->cs, 1, "502 Command not implemented.\n\r");
	return (EXIT_SUCCESS);
}

int 		s_retrieve(t_session *session)
{
	send_msg(session->cs, 1, "502 Command not implemented.\n\r");
	return (EXIT_SUCCESS);
}

int		s_list(t_session *session)
{
	pid_t			pid;

	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return (EXIT_FAILURE);
	if ((pid = fork()) == -1)
	{
		// set err code
		return (EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		redirect_output_fd(session->psv->cs);
		execv("/bin/ls", session->argv);
	}
	else if (pid == 0)
	{
		handel_sig(SIGINT);
		handel_sig(SIGCHLD);
	}
	return (EXIT_SUCCESS);
}
