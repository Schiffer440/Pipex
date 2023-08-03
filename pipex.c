/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:17:12 by adugain           #+#    #+#             */
/*   Updated: 2023/08/04 00:21:04 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	next_cmd(char *cmd, char **envp, char *std)
{
	pid_t	pid;
	int	p[2];

	if (pipe(p) == -1)
		ft_perror("Failed to pipe", 1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		dup2(p[1], 1);
		ft_exec(cmd, envp);
		close(p[1]);
	}
	else
	{	
		
		close(p[1]);
		if (ft_strncmp(std, "/dev/stdin", sizeof(std) - 1) != 0)
			dup2(p[0], 0);
		close(p[0]);
	}
}

static	void	last_cmd(char **av, char **envp, int *i)
{
	pid_t	pid;
	int	p[2];
	int	fd2;
	
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

void	pipex(int ac, char **av, char **envp)
{
	int	fd1;
	int	i;
	
	i = 2;
	fd1 = open(av[1], O_RDONLY);
	if (fd1 == -1)
		fd_perror(ft_strjoin("Error opening ", av[1]), fd1);
	else
	{
		dup2(fd1, 0);
		close(fd1);
	}
	while (i < ac - 2)
		next_cmd(av[i++], envp, av[1]);
	last_cmd(av, envp, &i);
}

static void	get_temp(int fd, char *end_of_file)
{
	char	*line;

	line = get_next_line(0);
	while (line != NULL)
	{
		if (ft_strncmp(line, end_of_file, sizeof(line) - 1) == 0
			&& sizeof(line) == sizeof(end_of_file))
			break ;
		else
		{
			ft_printf("heredoc>");
			ft_putstr_fd(line, fd);
			write(fd, "\n", 1);
			free(line);
			line = get_next_line(0);
		}
	}
	clean_the_mess();
	free(line);
}

static	void	last_cmd_heredoc(char **av, char **envp, int *i)
{
	pid_t		pid;
	int	p[2];
	int	fd1;

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

void	pipex_here_doc(int ac, char **av, char *end_of_file, char **envp)
{
	int		fd;
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
		next_cmd(av[i++], envp, av[1]);
	last_cmd_heredoc(av, envp, &i);
	unlink(temp);
}

int	main(int ac, char **av, char **envp)
{
	if (ac >= 5)
	{
		if (ft_strncmp(av[1], "here_doc", 8) == 0)
		{
			ft_printf("heredoc>");
			pipex_here_doc(ac, av, av[2], envp);
			return (0);
		}
		else
			pipex(ac, av, envp);
		while (wait(NULL) > 0)
			;
		return (0);
	}
	else
	{
		ft_printf("usage: ./pipex infile cmd1 cmd2 ... outfile");
		ft_printf(" heredoc usage: ./pipex here_doc cmd1 cmd2 ... outfile");
	}
	return (0);
}
