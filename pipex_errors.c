/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:38:01 by adugain           #+#    #+#             */
/*   Updated: 2023/07/31 14:50:53 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fd_perror(char *msg, int fd)
{
	fd = open("/tmp/cheat", O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
		ft_perror("Failed to hijack file", 127);
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	perror(msg);
}
