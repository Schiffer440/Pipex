/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 11:11:05 by adugain           #+#    #+#             */
/*   Updated: 2023/08/02 20:35:14 by adugain          ###   ########.fr       */
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

static char	*get_exec(char **paths, char **cl_cmd)
{
	char	*exec;
	char	*cl_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cl_path = ft_strjoin_pipex(paths[i], "/", 0);
		exec = ft_strjoin_pipex(cl_path, cl_cmd[0], 1);
		if (access(exec, F_OK | X_OK) == 0)
			break ;
		else
		{
			free(exec);
			exec = NULL;
			i++;
		}
	}
	return (exec);
}

void	ft_exec(char *cmd, char **envp)
{
	char	**cl_cmd;
	char	**paths;
	char	*exec;
	int	i;
	
	i = -1;
	exec = NULL;
	paths = get_paths(envp);
	cl_cmd = ft_split(cmd, ' ');
	while (paths[++i])
	{
		if (strncmp(paths[i], cmd, sizeof(cmd) - 1) == 0)
			exec = ft_strdup(cmd);
	}
	if (exec == NULL)
	{
		// cl_cmd = ft_split(cmd, ' ');
		exec = get_exec(paths, cl_cmd);
	}
	if (!exec || execve(exec, cl_cmd, envp) == -1)
		exec_error(paths, cl_cmd, &(*cmd));
}
