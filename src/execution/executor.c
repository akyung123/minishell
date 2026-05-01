/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 06:58:35 by akkim            ###   ########.fr       */
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
	char	*path_env;

	path_env = get_env_val(env->head, "PATH");
	if (path_env != NULL)
		pipex->paths = ft_split(path_env, ':');
	else
		pipex->paths = NULL;
	pipex->in = -1;
	pipex->out = -1;
	pipex->fd[0] = -1;
	pipex->fd[1] = -1;
	pipex->count = 0;
	pipex->envp = env->envp;
	pipex->pids = ft_calloc(1024, sizeof(pid_t));
}

int	executor_command_line(t_info_env *env, t_command_line *command_line)
{
	int		stat;
	t_pipex	*pipex;

	if (!command_line)
		return (env->exit_code);
	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	set_pipex(env, pipex);
	pipex->cmd_line = command_line;
	env->pipex = pipex;
	stat = executor_pipeline(env, command_line->pipeline, pipex);
	free_pipex(pipex);
	env->pipex = NULL;
	if (!command_line->next)
		return (env->exit_code);
	if (command_line->comm_oper == '&')
	{
		if (stat)
			executor_command_line(env, command_line->next);
		else
		{	
			while (command_line->next && command_line->comm_oper != '|')
				command_line = command_line->next;
			return (executor_command_line(env, command_line->next));
		}
	}
	else if (command_line->comm_oper == '|')
	{
		if (!stat)
			executor_command_line(env, command_line->next);
		else
		{
			while (command_line->next && command_line->comm_oper != '&')
				command_line = command_line->next;
			return (executor_command_line(env, command_line->next));
		}
	}
	return (1);
}
