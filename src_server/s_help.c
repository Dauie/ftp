/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_help.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 16:46:17 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/14 16:49:22 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/server.h"

int				s_help(t_session *session)
{
	if (send_msg(session->cs, 9, "FTP Server Usage:\n",
				"CWD <dir> - change working directory.\n",
				"HELP - Lists all supported commands\n",
				"LIST <path> - List files/directories in path\n",
				"PASV - Enter passive mode\n",
				"PWD - Prints working directory\n",
				"QUIT - Closes connection and quits program.\n",
				"RETR <path> - Retrieve file at path.\n",
				"STOR <path> - Store file at path.\n") == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
