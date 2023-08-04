/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 10:42:25 by adugain           #+#    #+#             */
/*   Updated: 2023/08/04 11:22:28 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	next_cmd(char *cmd, char **envp, char *std)
{
	pid_t	pid;
	int		p[2];

	(void)std;
	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		dup2(p[1], 1);
		close(p[1]);
		ft_exec(cmd, envp);
	}
	else
	{	
		close(p[1]);
		dup2(p[0], 0);
		close(p[0]);
	}
}

void	last_cmd(char **av, char **envp, int *i)
{
	pid_t	pid;
	int		p[2];
	int		fd2;

	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		fd2 = open(av[*i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd2 == -1)
			ft_perror(ft_strjoin(("Error opening "), av[*i + 1]), 1);
		dup2(fd2, 1);
		close(fd2);
		close(p[1]);
		ft_exec(av[*i], envp);
	}
	else
	{
		close(p[1]);
		dup2(p[0], 0);
		close(p[0]);
	}
}

void	last_cmd_heredoc(char **av, char **envp, int *i)
{
	pid_t	pid;
	int		p[2];
	int		fd1;

	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		fd1 = open(av[*i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd1 == -1)
			ft_perror(ft_strjoin(("Error opening "), av[*i + 1]), 1);
		dup2(fd1, 1);
		close(fd1);
		close(p[1]);
		ft_exec(av[*i], envp);
	}
	else
	{	
		close(p[1]);
		dup2(p[0], 0);
		close(p[0]);
	}
}
