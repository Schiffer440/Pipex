/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:17:12 by adugain           #+#    #+#             */
/*   Updated: 2023/07/19 21:49:01 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include "libft/libft.h"

char	*ft_strjoin_pipex(char *s1, char *s2, int token)
{
	char	*str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (0);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!str)
		return (0);
	while (s1[++i])
		str[i] = s1[i];
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	if (token == 1)
		free(s1);
	if (token == 2)
		free(s2);
	if (token == 3)
	{
		free(s1);
		free(s2);
	}
	return (str);
}

char	**get_paths(char **envp)
{
	int i;
	char **Mypath;
 
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH",4 ) == 0)
			break;
		else
			i++;
	}
	envp[i] += 5;
	Mypath = ft_split(envp[i], ':');
	return (Mypath);
}

void	ft_exec(char *cmd, char **envp)
{
	char	**arg;
	char	**paths;
	char	*exec;
	char	*cl_path;
	int	i;

	i = 0;
	arg = ft_split(cmd, ' ');
	paths = get_paths(envp);
	while (paths[i])
	{
		cl_path = ft_strjoin_pipex(paths[i], "/", 0);
		exec = ft_strjoin_pipex(cl_path, arg[0], 1);
		if (access(exec, F_OK | X_OK) == 0)
		{
			break;
		}
		else
		{
			free(exec);
			i++;
		}
	}
	execve(exec, arg, envp);
}

int	next_cmd(char *cmd, char **envp)
{
	int	pid;
	int	p[2];
	
	if (pipe(p) == -1)
		return (1);
	pid = fork();

	if (pid == 0)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[0]);
		close(p[1]);
		ft_exec(cmd, envp);
	}
	else
	{
		dup2(p[0], 0);
		close(p[0]);
		close(p[1]);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int	fd1;
	int 	fd2;
	int	i;
	
	i = 2;
	fd1 = open(av[1], O_RDONLY);
	if (fd1 == -1)
		perror("Pipex");
	else
	{
		dup2(fd1, 0);
		close(fd1);
	}
	while (i < ac - 2)
	{
		next_cmd(av[i++], envp);
	}
	fd2 = open(av[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd2, 1);
	ft_exec(av[i], envp);
}