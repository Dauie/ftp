/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_help_useage_quit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 13:43:24 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 13:43:24 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/client.h"

void	usage(char *str)
{
	printf("Usage: %s <addr> <port>\n", str);
	exit(-1);
}

int		c_quit(t_session *session)
{
	session->run = FALSE;
	return (SUCCESS);
}

int		c_help(t_session *session)
{
	recv_msg(session->sock, session->buff, &session->run);
	return (SUCCESS);
}
