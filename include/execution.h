/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:13:05 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 19:32:14 by akkim            ###   ########.fr       */
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

// setting function
void	set_process(t_pipex *pipex);
int		setting_command(t_info_env *env, t_pipex *pipex, t_simple_command *cmd);

pid_t	run_process(t_info_env *env, t_pipex *piepx,
			t_simple_command *simple_command);
void	run_cmd(t_info_env *env, t_pipex *pipex, t_simple_command *simple);
void	run_execve(t_pipex *pipex, t_simple_command *simple);

int		executor_command_line(t_info_env *env,
			t_command_line	*command_line);
int		executor_pipeline(t_info_env *env,
			t_pipeline *pipeline, t_pipex *pipex);
int		exec_single_builtin(t_info_env *env, t_pipeline *pl, t_pipex *px);	

// pipex function
void	free_pipex(t_pipex *pipex);
void	set_pipex(t_info_env *env, t_pipex *pipex);

// heredoc
int		here_doc(t_info_env *env, t_redirect *redirect);

char	*expand_tilde(char *token, t_info_env *env);

#endif