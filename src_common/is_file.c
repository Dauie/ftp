/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 22:41:00 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/07 22:41:41 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ftp.h"

int				is_file(char *pwd, char *file)
{
	struct stat	sd;
	char		*buff;

	if (!(buff = ft_strjoinn(3, pwd, "/", file)))
		return (FAILURE);
	if (stat(buff, &sd) == -1 || (S_ISREG(sd.st_mode) == 0))
	{
		free(buff);
		return (FAILURE);
	}
	free(buff);
	return (SUCCESS);
}
