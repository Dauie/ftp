#include "../incl/server.h"


int		redirect_output_fd(int fd)
{
	if (dup2(fd, STDERR_FILENO) < 0)
	{
		printf("[-]Error redirecting STDERR to file descriptor %d\n", fd);
		return (EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		printf("[-]Error redirecting STDOUT to file descriptor %d\n", fd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		change_dir(t_session *session)
{
	if (chdir(session->buff) == -1)
	{
		send_client_msg(session, 1, "451 Requested action aborted. Local error in processing.\n\r");
		return (EXIT_FAILURE);
	}
	send_client_msg(session, 1, "200 Working directory changed\n\r");
	return (EXIT_SUCCESS);
}

int		print_pwd(t_session *session)
{
	char	dir[256];
	char 	*res;

	if ((res = getcwd(dir, 255)) == NULL)
	{
		send_client_msg(session, 1, "451 Requested action aborted. Local error in processing.\n\r");
		return (EXIT_FAILURE);
	}
	send_client_msg(session, 3, "200 ", res, "\n\r");
	return (EXIT_SUCCESS);
}

int		list(t_session *session)
{
	pid_t			pid;
	struct rusage 	rusage;
	int 			wait_status;

	pid = 0;
	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return (EXIT_FAILURE);
	if ((pid = fork()) == -1)
	{
		// set err code
		return (EXIT_FAILURE);
	}
	else if (pid > 0)
		execv("/bin/ls", session->argv);
	else if (pid == 0)
	{
		if (wait4(pid, &wait_status, 0, &rusage ) == -1)
		{
			printf("[-]Error waiting on child process to complete\n");
			return(EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
