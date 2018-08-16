/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_dir_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:45:06 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/15 11:28:07 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

int					redirect_output_fd(int fd)
{
	if (dup2(fd, STDERR_FILENO) < 0)
	{
		printf("[-]Error redirecting STDERR to socket: %d\n", fd);
		return (FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		printf("[-]Error redirecting STDOUT to socket: %d\n", fd);
		return (FAILURE);
	}
	return (SUCCESS);
}

int					s_pwd(t_session *session)
{
	char			dir[256];
	char			*res;
	char			*hide_pwd;

	if (!(res = getcwd(dir, 255)) ||
			!(hide_pwd = ft_getenvar(session->env, "PWD", 3)))
	{
		send_msg(session->cs, 1, "451 Requested action aborted."
				" Local error in processing. \r\n");
		return (FAILURE);
	}
	ft_strcat(dir, "/");
	send_msg(session->cs, 3, "200 ", &res[ft_strlen(&hide_pwd[4])], " \r\n");
	free(hide_pwd);
	return (SUCCESS);
}

static int			directory_list(t_session *session)
{
	int				status;
	pid_t			pid;
	struct rusage	rusage;

	if ((pid = fork()) == -1)
	{
		send_msg(session->cs, 1, "451 Requested action aborted."
				" Local error in processing. \r\n");
		return (FAILURE);
	}
	else if (pid > 0)
	{
		wait4(pid, &status, 0, &rusage);
		close_passive(session, T_SVR);
		send_msg(session->cs, 1, "226 Closing data connection."
				" Requested file action successful. \r\n");
	}
	else if (pid == 0)
	{
		redirect_output_fd(session->psv->cs);
		execv("/bin/ls", session->argv);
	}
	return (SUCCESS);
}

int					s_list(t_session *session)
{
	if (session->mode == M_PSV)
		directory_list(session);
	else
		send_msg(session->cs, 1, "425 Can't open data"
				" connection. Use PASV.\r\n");
	return (SUCCESS);
}
