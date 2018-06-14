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

static char			*getenvar(char **env, char *qry, size_t qlen) {
	char *res;
	int i;

	i = -1;
	res = NULL;
	while (env[++i]) {
		if (ft_strncmp(env[i], qry, qlen) == 0 &&
			env[i][qlen] == '=')
			if (!(res = ft_strdup(env[i])))
				return (NULL);
	}
	return (res);
}

int		s_cwd(t_session *session)
{
	char	dir[256];
	char 	*start_path;
	char 	*cd;

    start_path = NULL;
    if (!(cd = getcwd(dir, 255)) || !(start_path = getenvar(session->env, "PWD", 3)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing. \r\n");
		return (EXIT_FAILURE);
	}
    if (session->argv[1] && ((ft_strcmp(cd, &start_path[4]) == 0 && ft_strcmp(session->argv[1], "..") == 0) ||
            ft_strncmp(session->argv[1], "/", 1) == 0 || ft_strncmp(session->argv[1], "~/", 2) == 0))
	{
        free(start_path);
		send_msg(session->cs, 1, "550 Requested action not taken. \r\n");
		return (EXIT_FAILURE);
	}
    cd = session->argv[1] ? session->argv[1] : &start_path[4];
    printf("%s", cd);
    if (chdir(cd) == -1)
    {
        free(start_path);
        send_msg(session->cs, 1, "550 Requested action not taken. File or directory not found. \r\n");
        return (EXIT_SUCCESS);
    }
    free(start_path);
	s_pwd(session);
	return (EXIT_SUCCESS);
}

int		s_pwd(t_session *session)
{
	char	dir[256];
	char 	*res;
	char	*hide_pwd;


	if (!(res = getcwd(dir, 255)) || !(hide_pwd = getenvar(session->env, "PWD", 3)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing. \r\n");
		return (EXIT_FAILURE);
	}
	strcat(dir, "/");
	send_msg(session->cs, 3, "200 ", &res[ft_strlen(&hide_pwd[4])], " \r\n");
    free(hide_pwd);
	return (EXIT_SUCCESS);
}

int		s_list(t_session *session)
{
    int             status;
	pid_t			pid;
    struct rusage rusage;

	if (session->mode == M_PSV)
	{
		if ((pid = fork()) == -1)
		{
            send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing. \r\n");
			return (EXIT_FAILURE);
		}
		else if (pid > 0)
		{
            wait4(pid, &status, 0, &rusage);
            close_passive(session, T_SVR);
            printf("closed connection\n");
            send_msg(session->cs, 1, "226 Closing data connection. Requested file action successful. \r\n");
		}
		else if (pid == 0)
        {
            redirect_output_fd(session->psv->cs);
            execv("/bin/ls", session->argv);
        }
	}
    else
        send_msg(session->cs, 1, "451 Requested action aborted. PASV not set. \r\n");
    return (EXIT_SUCCESS);
}
