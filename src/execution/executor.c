/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 02:10:03 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "parsing.h"
#include "execution.h"

void	free_pipex(t_pipex *pipex)
{
	free_split(pipex->paths);
	free(pipex->pids);
	free(pipex);
}

void	set_pipex(t_info_env *env, t_pipex *pipex)
{
	pipex->in = -1;
	pipex->out = -1;
	pipex->fd[0] = -1;
	pipex->fd[1] = -1;
	pipex->count = 0;
	pipex->paths = ft_split(get_env_val(env->head, "PATH"), ':');
	pipex->envp = env->envp;
	pipex->pids = ft_calloc(1024, sizeof(pid_t));
}

int	executor_command_line(t_info_env *env, t_command_line *command_line)
{
	int		stat;
	t_pipex	*pipex;

	if (!command_line)
		return (0);
	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	set_pipex(env, pipex);
	pipex->cmd_line = command_line;
	env->pipex = pipex;
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
	free_pipex(pipex);
	return (1);
}
