/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 01:41:02 by akkim            ###   ########.fr       */
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
	// open 실패 시, 에러 처리 안해둠(norm 25줄 문제)
	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	eof = ft_strjoin(redirect->filename, "\n");
	//printf("eof : %s\n", eof);
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

// redirect에 맞는 함수 생성
/* 파이프라인 실행에서 리디렉션이 있으면 리디렉션과 피이프로 둘 다 출력됨*/
// 위에건 ㄹㅇ 어케 하는거지? 
void	setting_command(t_pipex *pipex, t_simple_command *simple_command)
{
	// pre_red 연결
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
	// 단일 명령인 경우 builtin 처리함
	if (!pipeline->next && is_builtin(pipeline->simple_command->cmd))
	{
		builin_handler(env, pipeline->simple_command);
		if (env->exit_code != 0)
			return (0);
		return (1);
	}
	if (pipeline->next)
		pipe(pipex->fd);
	executor_simple_command(pipeline->simple_command, pipex);
	if (pipeline->next)
		executor_pipeline(env, pipeline->next, pipex);
	while (i < pipex->count)
	{
		waitpid(pipex->pids[i], &status, 0);
		if (WIFEXITED(status))
			env->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			env->exit_code = 128 + WTERMSIG(status);
		i++;
	}
	return (1);
}

int	executor_command_line(t_info_env *env, t_command_line *command_line)
{
	int		stat;
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	pipex->in = -1;
	pipex->out = -1;
	pipex->fd[0] = -1;
	pipex->fd[1] = -1;
	pipex->count = 0;
	pipex->paths = ft_split(get_env_val(env->head, "PATH"), ':');
	pipex->envp = env->envp;
	pipex->pids = (pid_t *)malloc(sizeof(pid_t) * 1024);
	if (!command_line)
		return (0);
	stat = executor_pipeline(env, command_line->pipeline, pipex);
	if (command_line->comm_oper == '&')
	{
		if (stat) // 성공하면
			executor_command_line(env, command_line->next);
	}
	else if (command_line->comm_oper == '|')
	{
		if (!stat) // 실패하면
			executor_command_line(env, command_line->next);
	}
	return (1);
}
