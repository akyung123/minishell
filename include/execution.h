/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:13:05 by akkim             #+#    #+#             */
/*   Updated: 2026/04/28 22:43:19 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <fcntl.h>

typedef struct l_pipex
{
	int		pid;
	int		count;
	int		in;
	int		out;
	int		fd[2];
	pid_t	*pids;
	char	**paths;
	char	**envp;
}	t_pipex;

void	set_process(t_pipex *pipex);
pid_t	run_process(t_info_env *env, t_pipex *piepx, t_simple_command *simple_command);
void	run_cmd(t_pipex *pipex, t_simple_command *simple);
int		executor_pipeline(t_info_env *env,
			t_pipeline *pipeline, t_pipex *pipex);

#endif