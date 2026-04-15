/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/04/15 21:13:12 by akkim            ###   ########.fr       */
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
		pipex->in = open_file_red(simple_command->pre_red);
	if (pipex->in == -1)
		perror(simple_command->pre_red->filename);
	if (simple_command->suff_red)
		pipex->out = open_file_red(simple_command->suff_red);
	if (pipex->out == -1)
	{
		perror(simple_command->suff_red->filename);
		close(pipex->in);
		close(pipex->out);
		free(pipex);
		exit(1);
		// exit용 함수 새로 만들어야함
		// free해야할 메모리가 많음
	}
}

// simpel_command 실행 함수
int	executor_simple_command(t_info_env *env, t_simple_command *simple_command)
{
	int		pid;
	int		status;
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	pipe(pipex->fd);
	// 매번 PATH를 계산하는 비효율적인 과정이 있음. 추후 수정할 것.
	pipex->paths = ft_split(get_env_val(env->head, "PATH"), ':');
	pipex->envp = env->envp;
	setting_command(pipex, simple_command);
	pid = run_process(pipex, simple_command);
	if (pid > 0)
		waitpid(pid, &status, 0);
	return (pid); //반환값 생성 추가해야함.
}

int	executor_pipeline(t_info_env *env, t_pipeline *pipeline)
{
	int		state;

	// pipeline->pipeline이 없을 때까지 진행해야한다. 
	// 모든 pipeline->simple_command는 프로세스로 분리해서 실행한다
	state = 0;
	if (!pipeline)
		return (0);
	// state로 종료 코드 전달 받기
	state = executor_simple_command(env, pipeline->simple_command);
	if (state)
	{
		// wait() : 프로세스 대기 함수 사용
		executor_pipeline(env, pipeline->next);
	}
	return (1);
}

int	executor_command_line(t_info_env *env, t_command_line *command_line)
{
	int	state;

	state = 0;
	if (!command_line)
		return (0);
	state = executor_pipeline(env, command_line->pipeline);
	if (command_line->comm_oper == '&')
	{
		if (state) // 성공하면
			executor_command_line(env, command_line->next);
	}
	else if (command_line->comm_oper == '|')
	{
		if (!state) // 실패하면
			executor_command_line(env, command_line->next);
	}
	return (1);
}
