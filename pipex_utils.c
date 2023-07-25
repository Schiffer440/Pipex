/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 11:11:05 by adugain           #+#    #+#             */
/*   Updated: 2023/07/25 11:58:07 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_token(char *s1, char *s2, int token)
{
	if (token == 1)
		free(s1);
	if (token == 2)
		free(s2);
	if (token == 3)
	{
		free(s1);
		free(s2);
	}
}

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
		str[i++] = s2[j++];
	str[i] = '\0';
	free_token(s1, s2, token);
	return (str);
}

static char	**get_paths(char **envp)
{
	int		i;
	char	**mypath;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			break ;
		else
			i++;
	}
	envp[i] += 5;
	mypath = ft_split(envp[i], ':');
	return (mypath);
}

void	ft_exec(char *cmd, char **envp)
{
	char	**arg;
	char	**paths;
	char	*exec;
	char	*cl_path;
	int		i;

	i = 0;
	arg = ft_split(cmd, ' ');
	paths = get_paths(envp);
	while (paths[i])
	{
		cl_path = ft_strjoin_pipex(paths[i], "/", 0);
		exec = ft_strjoin_pipex(cl_path, arg[0], 1);
		if (access(exec, F_OK | X_OK) == 0)
		{
			break ;
		}
		else
		{
			free(exec);
			i++;
		}
	}
	execve(exec, arg, envp);
}
