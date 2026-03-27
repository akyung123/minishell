/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:11:14 by akkim             #+#    #+#             */
/*   Updated: 2025/12/30 20:43:51 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"
# include "get_next_line.h"

typedef struct l_pipex
{
	int		i;
	int		pid;
	int		in;
	int		out;
	int		fd[2];
	char	**argv;
	char	**paths;
	char	**envp;
}	t_pipex;

typedef struct l_exit
{
	int		status;
	int		last_pid;
	int		last_exit_code;
}	t_exit;

void	setting_pipex(t_pipex *pipex, int argc, char **argv, char **envp);
int		here_doc(char **argv, int prev_fd);
pid_t	run_process(t_pipex *pipex, int argc, char *cmd);
void	run_cmd(t_pipex *pipex, char *cmd);
char	**free_split(char **str);

#endif
