/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:17:12 by adugain           #+#    #+#             */
/*   Updated: 2023/08/01 18:02:01 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	next_cmd(char *cmd, char **envp)
{
	int	pid;
	int	p[2];

	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 127);
	pid = fork();
	if (pid == 0)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);
		close(p[0]);
		ft_exec(cmd, envp);
	}
	else
	{
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		close(p[1]);
	}
}

void	pipex(int ac, char **av, char **envp)
{
	int	fd1;
	int	fd2;
	int	i;

	i = 2;
	fd1 = open(av[1], O_RDONLY);
	if (fd1 == -1)
		fd_perror(ft_strjoin("Error opening ", av[1]), fd1);
	else
	{
		dup2(fd1, STDIN_FILENO);
		close(fd1);
	}
	while (i < ac - 2)
	{
		next_cmd(av[i++], envp);
	}
	fd2 = open(av[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd2 == -1)
		ft_perror("Error opening file", 127);
	dup2(fd2, STDOUT_FILENO);
	close(fd2);
	ft_exec(av[i], envp);
}

static void	get_temp(int fd, char *end_of_file)
{
	char	*line;

	line = get_next_line(0);
	ft_printf("heredoc>");
	while (line != NULL)
	{
		if (ft_strncmp(line, end_of_file, sizeof(line) - 1) == 0
			&& sizeof(line) == sizeof(end_of_file))
			break ;
		ft_putstr_fd(line, fd);
		write(fd, "\n", 1);
		free(line);
		line = get_next_line(0);
		ft_printf("heredoc>");
	}
	clean_the_mess();
	free(line);
}

void	pipex_here_doc(int ac, char **av, char *end_of_file, char **envp)
{
	int		fd;
	int		fd1;
	char	*temp;
	int		i;

	i = 3;
	temp = "/tmp/temp";
	fd = open(temp, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		perror("Here_doc error");
	get_temp(fd, end_of_file);
	close(fd);
	fd = open(temp, O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
	while (i < ac - 2)
	{
		next_cmd(av[i++], envp);
	}
	fd1 = open(av[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(fd1, STDOUT_FILENO);
	close(fd1);
	unlink(temp);
	ft_exec(av[i], envp);
}

int	main(int ac, char **av, char **envp)
{
	if (ac >= 4)
	{
		if (ft_strncmp(av[1], "here_doc", 8) == 0)
		{
			ft_printf("heredoc>");
			pipex_here_doc(ac, av, av[2], envp);
			return (0);
		}
		else
			pipex(ac, av, envp);
		while (wait(NULL) != 0 || errno != ECHILD)
			;
		return (0);
	}
	else
		write(1, "\n", 1);
	return (0);
}
