#include "../incl/ftp.h"


int		redirect_output_fd(int fd)
{
	if (dup2(fd, STDERR_FILENO) < 0)
	{
		printf("[-]Error redirecting STDERR to file descriptor %d\n", fd);
		return (EXIT_FAIL);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		printf("[-]Error redirecting STDOUT to file descriptor %d\n", fd);
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

int		change_dir(t_session *session, char *dir)
{
	if (chdir(dir) == -1)
	{
		//set error code
		return (EXIT_FAIL);
	}
	//update saved env
	//send new directory to client

	return (EXIT_SUCCESS);
}

int		print_cwd(t_session *session)
{
	char	dir[256];
	char 	*res;

	if ((res = getcwd(dir, 255)) == NULL)
	{
		//set error code
		return (EXIT_FAIL);
	}
	write(session->cs, res, ft_strlen(dir));
	return (EXIT_SUCCESS);
}

int		list(t_session *session)
{
	pid_t			pid;
	struct rusage 	rusage;
	int 			wait_status;

	pid = 0;
	if (!(session->argv = ft_strsplit(session->buff, ' ')))
		return (EXIT_FAIL);
	if ((pid = fork()) == -1)
	{
		// set err code
		return (EXIT_FAIL);
	}
	else if (pid > 0)
		execv("/bin/ls", session->argv);
	else if (pid == 0)
	{
		if (wait4(pid, &wait_status, 0, &rusage ) == -1)
		{
			printf("[-]Error waiting on child process to complete\n");
			return(EXIT_FAIL);
		}
	}
	return (EXIT_SUCCESS);
}