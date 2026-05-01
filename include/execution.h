/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:13:05 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 01:19:17 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <fcntl.h>
# include <sys/stat.h>
# include "parsing.h"

typedef struct l_pipex
{
	int				pid;
	int				count;
	int				in;
	int				out;
	int				stdin_backup;
	int				stdout_backup;
	int				fd[2];
	int				pipeline_status;
	pid_t			*pids;
	char			**paths;
	char			**envp;
	t_command_line	*cmd_line;
}	t_pipex;

void	set_process(t_pipex *pipex);
pid_t	run_process(t_info_env *env, t_pipex *piepx, t_simple_command *simple_command);
void	run_cmd(t_info_env *env, t_pipex *pipex, t_simple_command *simple);
int		executor_pipeline(t_info_env *env,
			t_pipeline *pipeline, t_pipex *pipex);

void	free_pipex(t_pipex *pipex);
void	set_pipex(t_info_env *env, t_pipex *pipex);

#endif