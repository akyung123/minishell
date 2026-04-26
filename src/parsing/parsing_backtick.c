/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_backtick.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 00:58:26 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 01:10:24 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
#include "execution.h"
#include "quote.h"

char	*read_from_pipe(int fd)
{
	char	buffer[1024];
	int		bytes_read;
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (1)
	{
		bytes_read = read(fd, buffer, 1023);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(result, buffer);
		free(result);
		result = tmp;
	}
	return (result);
}

void	replace_newline_to_space(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == '\n')
			str[i] = ' ';
		i++;
	}
}

// 1. 치환 결과물에 있는 따옴표를 특수문자로 숨김
void	hide_expanded_quotes(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == '\'')
			str[i] = 1; // 안 쓰는 아스키코드 1번으로 변장
		else if (str[i] == '\"')
			str[i] = 2; // 안 쓰는 아스키코드 2번으로 변장
		i++;
	}
}

// 2. 파싱이 다 끝난 후 원래 따옴표로 복구
void	restore_expanded_quotes(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == 1)
			str[i] = '\'';
		else if (str[i] == 2)
			str[i] = '\"';
		i++;
	}
}

char	*execute_backtick(char *inner_cmd, t_info_env *env)
{
	pid_t		pid;
	char		*result;
	t_pipex		*pipex;
	t_pipeline	*sub;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (pipe(pipex->fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		close(pipex->fd[0]);
		dup2(pipex->fd[1], STDOUT_FILENO);
		close(pipex->fd[1]);
		sub = parsing_pipeline(env, inner_cmd);
		if (sub)
			executor_pipeline(env, sub, pipex);
		exit(0);
	}
	close(pipex->fd[1]);
	result = read_from_pipe(pipex->fd[0]);
	close(pipex->fd[0]);
	waitpid(pid, NULL, 0);
	replace_newline_to_space(result);
	hide_expanded_quotes(result);
	return (result);
}

void	process_backticks(char **line, t_info_env *env)
{
	char	*res;
	char	*inner;
	char	*out;
	int		i;
	int		start;
	int		state;

	res = ft_strdup("");
	i = 0;
	state = 0;
	while ((*line) && (*line)[i])
	{
		update_quote_state((*line)[i], &state);
		if ((*line)[i] == '`' && !(state & 1))
		{
			start = ++i;
			while ((*line)[i] && (*line)[i] != '`')
				i++;
			inner = ft_substr(*line, start, i - start);
			out = execute_backtick(inner, env);
			free(inner);
			if (out)
			{
				res = ft_strjoin_free(res, out);
				free(out);
			}
			if ((*line)[i] == '`')
			{
				update_quote_state((*line)[i], &state);
				i++;
			}
		}
		else
		{
			res = append_char(res, (*line)[i]);
			i++;
		}
	}
	free(*line);
	*line = res;
}
