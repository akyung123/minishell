/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 07:52:11 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 06:49:07 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "execution.h"
#include "minishell.h"
#include "quote.h"

int	here_doc(t_info_env *env, t_redirect *redirect)
{
	int		fd;
	char	*eof;
	char	*line;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	eof = ft_strjoin(redirect->filename, "\n");
	if (isatty(STDIN_FILENO))
		write(1, "> ", 2);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(redirect->filename, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (redirect->flag != 1)
			expand_env(&line, env);
		if (!ft_strncmp(line, eof, ft_strlen(eof)))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("> ", 2);
	}
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	unlink(".here_doc");
	return (free(eof), fd);
}

// redirect에 맞게 파일 열기
int	open_file_red(t_info_env *env, t_redirect *redirect)
{
	int	fd;

	if (ft_strcmp(redirect->type, "<<") == 0)
		fd = here_doc(env, redirect);
	else if (ft_strcmp(redirect->type, ">>") == 0)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strcmp(redirect->type, "<") == 0)
		fd = open(redirect->filename, O_RDONLY);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

// red->next가 없어질 때까지 반복해서 열기
void	setting_command(t_info_env *env, t_pipex *pipex, t_simple_command *simple_command)
{
	t_redirect	*tmp;

	tmp = simple_command->red;
	while (tmp)
	{
		if (tmp && tmp->type && (!ft_strcmp(tmp->type, "<<") || !ft_strcmp(tmp->type, "<")))
		{
			pipex->in = open_file_red(env, tmp);
			if (pipex->in == -1)
				perror(tmp->filename);
		}
		else
		{
			pipex->out = open_file_red(env, tmp);
			if (pipex->out == -1)
			{
				perror(tmp->filename);
				exit(1);
			}
		}
		tmp = tmp->next;
	}
}

// simpel_command 실행 함수
int	executor_simple_command(t_info_env *env, t_simple_command *simple_command, t_pipex *pipex)
{
	int		pid;

	setting_command(env, pipex, simple_command);
	pid = run_process(env, pipex, simple_command);
	pipex->pids[pipex->count] = pid;
	pipex->count++;
	return (pid);
}
// exeutor builtin 
// run process랑 동일하게 실행함
// 실행 후 
int	executor_pipeline(t_info_env *env, t_pipeline *pipeline, t_pipex *pipex)
{
	int					stdout_backup;
	int					status;
	int					i;
	int					is_root;
	int					ac;
	t_pipeline			*last_pipe;
	t_simple_command	*new;
	t_simple_command	*tmp;

	i = 0;
	is_root = (pipex->count == 0);
	stdout_backup = dup(STDOUT_FILENO);
	if (!pipeline)
		return (0);
	refine_line(&(pipeline->simple_command->line), env);
	new = parsing_simple_command(env, pipeline->simple_command->line);
	tmp = pipeline->simple_command;
	pipeline->simple_command = new;
	free_simple_command(tmp);
	if (is_root && !pipeline->next && is_builtin(pipeline->simple_command->cmd))
	{
		setting_command(env, pipex, pipeline->simple_command);
		set_process(pipex);
		builtin_handler(env, pipeline->simple_command);
		ac = 0;
		while (pipeline->simple_command->args && pipeline->simple_command->args[ac])
			ac++;
		if (ac > 0)
			update_last_arg(env, pipeline->simple_command->args[ac - 1]);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
		pipex->pipeline_status = (env->exit_code == 0);
		return (pipex->pipeline_status);
	}
	if (pipeline->next)
		pipe(pipex->fd);
	executor_simple_command(env, pipeline->simple_command, pipex);
	if (pipeline->next)
		executor_pipeline(env, pipeline->next, pipex);
	if (is_root)
	{
		while (i < pipex->count)
		{
			waitpid(pipex->pids[i++], &status, 0);
			if (WIFEXITED(status))
				env->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				env->exit_code = 128 + WTERMSIG(status);
		}
		last_pipe = pipeline;
		while (last_pipe->next)
			last_pipe = last_pipe->next;
		ac = 0;
		if (last_pipe->simple_command && last_pipe->simple_command->args)
		{
			while (last_pipe->simple_command->args[ac])
				ac++;
			if (ac > 0)
				update_last_arg(env, last_pipe->simple_command->args[ac - 1]);
		}
		pipex->pipeline_status = (env->exit_code == 0);
	}
	return (pipex->pipeline_status);
}
