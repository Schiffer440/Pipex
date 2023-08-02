/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duplicate_fds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 17:30:17 by adugain           #+#    #+#             */
/*   Updated: 2023/08/02 18:27:29 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	dup_pipein(int *p)
{
	if (dup2(p[1], STDOUT_FILENO) == -1)
		exit (127);
	if (close(p[1]) == -1)
		exit (127);
	if (close(p[0]) == -1)
		exit (127);
}

void	dup_pipeout(int *p)
{
	if (dup2(p[0], STDIN_FILENO) == -1)
		exit (127);
	if (close(p[0]) == -1)
		exit (127);
	if (close(p[1]) == -1)
		exit (127);
}