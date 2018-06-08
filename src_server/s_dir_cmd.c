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
	char 	*res;

	if (!(res = getcwd(dir, 255)) || !(start_path = getenvar(session->env, "PWD", 3)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing.\n\r");
		return (EXIT_FAILURE);
	}
	if ((ft_strcmp(res, &start_path[4]) == 0 && ft_strcmp(session->argv[1], "..") == 0) ||
			chdir(session->argv[1]) == -1)
	{
		send_msg(session->cs, 1, "550 Requested action not taken. File unavailable\n\r");
		return (EXIT_FAILURE);
	}
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
		send_msg(session->cs, 1, "451 Requested action aborted. Local error in processing.\n\r");
		return (EXIT_FAILURE);
	}
	strcat(dir, "/");
	send_msg(session->cs, 3, "200 ", &res[ft_strlen(&hide_pwd[4])], "\n\r");
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
