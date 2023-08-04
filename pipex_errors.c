/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:38:01 by adugain           #+#    #+#             */
/*   Updated: 2023/08/04 10:57:21 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fd_perror(char *msg, int fd)
{
	fd = open("/tmp/cheat", O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
	{	
		perror("Failed to hijack file");
		exit(127);
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		unlink("/tmp/cheat");
	}
	perror(msg);
	free(msg);
}

void	clean_the_mess(void)
{
	int	fd_clean;

	fd_clean = open("/tmp/clean", O_RDONLY | O_CREAT, 0644);
	if (fd_clean == -1)
	{	
		perror("Failed to clean heredoc...");
		exit(1);
	}
	else
	{
		get_next_line(fd_clean);
		close(fd_clean);
		unlink("/tmp/clean");
	}
}

void	exec_error(char **paths, char **cmd, char *com)
{
	char	*msg;

	msg = ft_strjoin_pipex(ft_strdup("command not found: "), ft_strdup(com), 3);
	ft_free_tab_c(paths);
	ft_free_tab_c(cmd);
	perror(msg);
	free(msg);
	exit(127);
}
