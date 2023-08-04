/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 10:42:25 by adugain           #+#    #+#             */
/*   Updated: 2023/08/04 12:26:55 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	open_out_fd(char **av, int fd, int *i)
{
	fd = open(av[*i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ft_perror(ft_strjoin(("Error opening "), av[*i + 1]), 1);
	return (fd);
}

static int	open_out_fd_heredoc(char **av, int fd, int *i)
{
	fd = open(av[*i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		ft_perror(ft_strjoin(("Error opening "), av[*i + 1]), 1);
	return (fd);
}

void	next_cmd(char *cmd, char **envp, char *stdin)
{
	pid_t	pid;
	int		p[2];

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
		if (ft_strncmp(stdin, "/dev/stdin", sizeof(stdin) - 1) != 0)
			dup2(p[0], 0);
		close(p[0]);
	}
}

void	last_cmd(char **av, char **envp, int *i)
{
	pid_t	pid;
	int		p[2];
	int		fd2;

	fd2 = 0;
	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		if (ft_strncmp(av[*i + 1], "/dev/stdout", sizeof(av[*i + 1]) - 1) != 0)
			dup2(p[1], 1);
		close(p[1]);
		fd2 = open_out_fd(av, fd2, i);
		dup2(fd2, 1);
		close(fd2);
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

	fd1 = 0;
	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		if (ft_strncmp(av[*i + 1], "/dev/stdout", sizeof(av[*i + 1]) - 1) != 0)
			dup2(p[1], 1);
		close(p[1]);
		fd1 = open_out_fd_heredoc(av, fd1, i);
		dup2(fd1, 1);
		close(fd1);
		ft_exec(av[*i], envp);
	}
	else
	{	
		close(p[1]);
		dup2(p[0], 0);
		close(p[0]);
	}
}
