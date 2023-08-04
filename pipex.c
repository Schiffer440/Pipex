/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:17:12 by adugain           #+#    #+#             */
/*   Updated: 2023/08/04 11:21:00 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		ft_printf(">usage: ./pipex infile cmd1 cmd2 ... outfile\n");
		ft_printf(">heredoc usage: ./pipex here_doc cmd1 cmd2 ... outfile");
	}
	return (0);
}
