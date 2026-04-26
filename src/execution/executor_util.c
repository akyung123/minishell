/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 07:52:11 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 07:58:26 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "execution.h"
#include "minishell.h"

int	here_doc(t_redirect *redirect)
{
	int		fd;
	char	*eof;
	char	*line;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	eof = ft_strjoin(redirect->filename, "\n");
	write(1, "> ", 2);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		if (!ft_strncmp(line, eof, ft_strlen(eof)))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		write(1, "> ", 2);
	}
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	unlink(".here_doc");
	return (free(eof), fd);
}

// redirect에 맞게 파일 열기
int	open_file_red(t_redirect *redirect)
{
	int	fd;

	if (ft_strcmp(redirect->type, "<<") == 0)
		fd = here_doc(redirect);
	else if (ft_strcmp(redirect->type, ">>") == 0)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strcmp(redirect->type, "<") == 0)
		fd = open(redirect->filename, O_RDONLY);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

void	setting_command(t_pipex *pipex, t_simple_command *simple_command)
{
	if (simple_command->pre_red)
	{
		pipex->in = open_file_red(simple_command->pre_red);
		if (pipex->in == -1)
			perror(simple_command->pre_red->filename);
	}
	if (simple_command->suff_red)
	{
		pipex->out = open_file_red(simple_command->suff_red);
		if (pipex->out == -1)
		{
			perror(simple_command->suff_red->filename);
			exit(1);
		}
	}
}

// simpel_command 실행 함수
int	executor_simple_command(t_simple_command *simple_command, t_pipex *pipex)
{
	int		pid;

	setting_command(pipex, simple_command);
	pid = run_process(pipex, simple_command);
	pipex->count++;
	return (pid);
}

int	executor_pipeline(t_info_env *env, t_pipeline *pipeline, t_pipex *pipex)
{
	int		status;
	int		i;

	i = 0;
	if (!pipeline)
		return (0);
	if (!pipeline->next && is_builtin(pipeline->simple_command->cmd))
	{
		builin_handler(env, pipeline->simple_command);
		return (env->exit_code == 0);
	}
	if (pipeline->next)
		pipe(pipex->fd);
	executor_simple_command(pipeline->simple_command, pipex);
	if (pipeline->next)
		executor_pipeline(env, pipeline->next, pipex);
	while (i < pipex->count)
	{
		waitpid(pipex->pids[i++], &status, 0);
		if (WIFEXITED(status))
			env->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			env->exit_code = 128 + WTERMSIG(status);
	}
	return (1);
}
