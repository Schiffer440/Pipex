/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adugain <adugain@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 11:04:39 by adugain           #+#    #+#             */
/*   Updated: 2023/08/04 11:21:28 by adugain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <signal.h>

void	pipex(int ac, char **av, char **envp);
void	pipex_here_doc(int ac, char **av, char *endname, char **envp);
char	*ft_strjoin_pipex(char *s1, char *s2, int token);
void	ft_exec(char *cmd, char **envp);
void	fd_perror(char *msg, int fd);
void	clean_the_mess(void);
void	exec_error(char **paths, char **cmd, char *com);
void	next_cmd(char *cmd, char **envp, char *std);
void	last_cmd(char **av, char **envp, int *i);
void	last_cmd_heredoc(char **av, char **envp, int *i);

#endif