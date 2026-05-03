/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 07:52:11 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 19:38:03 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "quote.h"

static int	handle_setting_fail(t_pipex *pipex)
{
	if (pipex->in > 2)
		close(pipex->in);
	if (pipex->out > 2)
		close(pipex->out);
	if (pipex->fd[1] > 2)
		close(pipex->fd[1]);
	pipex->in = pipex->fd[0];
	return (-1);
}

// simpel_command 실행 함수
int	executor_simple_command(t_info_env *env,
	t_simple_command *cmd, t_pipex *pipex)
{
	int	pid;

	if (!cmd)
	{
		env->exit_code = 0;
		return (-1);
	}
	if (!setting_command(env, pipex, cmd))
		return (handle_setting_fail(pipex));
	if (!cmd->cmd || !cmd->args || !cmd->args[0])
	{
		env->exit_code = 0;
		return (-1);
	}
	pid = run_process(env, pipex, cmd);
	pipex->pids[pipex->count] = pid;
	pipex->count++;
	return (pid);
}

static int	parse_pipeline_cmd(t_info_env *env, t_pipeline *pipeline)
{
	t_simple_command	*new;
	t_simple_command	*tmp;

	new = parsing_simple_command(env, pipeline->simple_command->line);
	if (!new)
		return (0);
	tmp = pipeline->simple_command;
	pipeline->simple_command = new;
	free_simple_command(tmp);
	return (1);
}

static void	wait_pipeline(t_info_env *env, t_pipeline *pl, t_pipex *pipex)
{
	int	i;
	int	status;
	int	ac;

	i = -1;
	while (++i < pipex->count)
	{
		waitpid(pipex->pids[i], &status, 0);
		if (WIFEXITED(status))
			env->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			env->exit_code = 128 + WTERMSIG(status);
	}
	while (pl->next)
		pl = pl->next;
	ac = 0;
	if (pl->simple_command && pl->simple_command->args)
	{
		while (pl->simple_command->args[ac])
			ac++;
		if (ac > 0)
			update_last_arg(env, pl->simple_command->args[ac - 1]);
	}
	pipex->pipeline_status = (env->exit_code == 0);
}

// exeutor builtin 
// run process랑 동일하게 실행함
// 실행 후 
int	executor_pipeline(t_info_env *env, t_pipeline *pipeline, t_pipex *pipex)
{
	int	is_root;

	if (!pipeline)
		return (0);
	is_root = (pipex->count == 0);
	if (!parse_pipeline_cmd(env, pipeline))
		return (pipex->pipeline_status);
	if (is_root && !pipeline->next && is_builtin(pipeline->simple_command->cmd))
		return (exec_single_builtin(env, pipeline, pipex));
	if (pipeline->next)
		pipe(pipex->fd);
	executor_simple_command(env, pipeline->simple_command, pipex);
	if (pipeline->next)
		executor_pipeline(env, pipeline->next, pipex);
	if (is_root)
		wait_pipeline(env, pipeline, pipex);
	return (pipex->pipeline_status);
}
