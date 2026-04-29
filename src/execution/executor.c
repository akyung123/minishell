/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/04/29 14:00:41 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "execution.h"
#include "minishell.h"

void	set_pipex(t_info_env *env, t_pipex *pipex)
{
	pipex->in = -1;
	pipex->out = -1;
	pipex->fd[0] = -1;
	pipex->fd[1] = -1;
	pipex->count = 0;
	pipex->paths = ft_split(get_env_val(env->head, "PATH"), ':');
	pipex->envp = env->envp;
	pipex->pids = (pid_t *)malloc(sizeof(pid_t) * 1024);
}

int	executor_command_line(t_info_env *env, t_command_line *command_line)
{
	int		stat;
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	set_pipex(env, pipex);
	if (!command_line)
		return (0);
	stat = executor_pipeline(env, command_line->pipeline, pipex);
	if (command_line->comm_oper == '&')
	{
		if (stat)
			executor_command_line(env, command_line->next);
		else
			executor_command_line(env, command_line->next->next);
	}
	else if (command_line->comm_oper == '|')
	{
		if (!stat)
			executor_command_line(env, command_line->next);
		else
			executor_command_line(env, command_line->next->next);
	}
	return (1);
}
