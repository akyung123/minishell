/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 13:41:48 by akkim            ###   ########.fr       */
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

static int	handle_logical_oper(t_info_env *env, t_command_line *cmd, int stat)
{
	if (cmd->comm_oper == '&')
	{
		if (stat)
			return (executor_command_line(env, cmd->next));
		while (cmd->next && cmd->comm_oper != '|')
			cmd = cmd->next;
		return (executor_command_line(env, cmd->next));
	}
	if (cmd->comm_oper == '|')
	{
		if (!stat)
			return (executor_command_line(env, cmd->next));
		while (cmd->next && cmd->comm_oper != '&')
			cmd = cmd->next;
		return (executor_command_line(env, cmd->next));
	}
	return (1);
}

int	executor_command_line(t_info_env *env, t_command_line *cmd)
{
	int		stat;
	t_pipex	*pipex;

	if (!cmd)
		return (env->exit_code);
	pipex = malloc(sizeof(t_pipex));
	set_pipex(env, pipex);
	pipex->cmd_line = cmd;
	env->pipex = pipex;
	stat = executor_pipeline(env, cmd->pipeline, pipex);
	free_pipex(pipex);
	env->pipex = NULL;
	if (!cmd->next)
		return (env->exit_code);
	return (handle_logical_oper(env, cmd, stat));
}
