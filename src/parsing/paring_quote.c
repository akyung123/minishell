/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:34:25 by akkim             #+#    #+#             */
/*   Updated: 2026/04/26 23:47:16 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

// // 비트 마스크로 state update
void	update_quote_state(char c, int *state)
{
	if (c == '\'' && !(*state & 2) && !(*state & 4))
		*state ^= 1;
	else if (c == '\"' && !(*state & 1) && !(*state & 4))
		*state ^= 2;
	else if (c == '`' && !(*state & 1))
		*state ^= 4;
}

// 입력받아서 합치는 함수
int	plus_line(char **line)
{
	char	*new;
	char	*tmp1;
	char	*tmp2;

	new = readline("> ");
	if (!new)
		return (0);
	tmp1 = ft_strjoin(*line, "\n");
	if (!tmp1)
	{
		free(new);
		return (0);
	}
	tmp2 = ft_strjoin(tmp1, new);
	free(*line);
	free(new);
	*line = tmp2;
	return (1);
}

// "가 있는 곳 까지 땡기는 함수
// >>> 현재 안쓰고 있음 <<< -> 추후 삭제 요청
int	skip_quote(char **line, int start_i, char type)
{
	int	i;
	int	res;

	i = start_i + 1;
	while (1)
	{
		if (!*line)
			return (-1);
		if ((*line)[i] == '\0')
		{
			if (!plus_line(line))
				return (-1);
			i = 0;
		}
		else if ((type == '\"' && (*line)[i] == '`'))
		{
			res = skip_quote(line, i, '`');
			if (res == -1)
				return (-1);
			i = res + 1;
		}
		else if ((*line)[i] == type)
			return (i);
		else
			i++;
	}
}

int	skip_spaces(char *line, int i)
{
	while (line[i] && line[i] == ' ')
		i++;
	return (i);
}

// 에러 메시지 출력 기본 헬퍼
int	syntax_err_msg(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

// 에러가 발생한 위치의 토큰을 읽어서 출력하는 함수
int	print_unexpected_token(char *line, int idx)
{
	char	token[3];

	if (line[idx] == '\0')
		return (syntax_err_msg("newline"));
	token[0] = line[idx];
	token[1] = '\0';
	token[2] = '\0';
	if ((line[idx] == '<' || line[idx] == '>') && line[idx] == line[idx + 1])
		token[1] = line[idx + 1];
	return (syntax_err_msg(token));
}

int	check_invalid_operator(char *line, int i)
{
	int	next;

	if (line[i] == '|')
	{
		next = skip_spaces(line, i + 1);
		if (line[next] == '|' || line[next] == '\0')
			return (print_unexpected_token(line, next));
	}
	else if (line[i] == '<' || line[i] == '>')
	{
		if (line[i + 1] == line[i])
			i++;
		next = skip_spaces(line, i + 1);
		if (line[next] == '\0' || line[next] == '<' || \
			line[next] == '>' || line[next] == '|')
			return (print_unexpected_token(line, next));
	}
	return (0);
}

int	check_unclosed_quotes(char *line)
{
	int	i;
	int	state;

	i = 0;
	state = 0;
	while (line[i])
	{
		update_quote_state(line[i], &state);
		i++;
	}
	if (state != 0)
		return (1);
	return (0);
}


int	check_syntax_error(char *line)
{
	int	i;
	int	state;

	if (check_unclosed_quotes(line))
		return (syntax_err_msg("unclosed quotes"));
	i = skip_spaces(line, 0);
	if (line[i] == '|')
		return (print_unexpected_token(line, i));
	state = 0;
	while (line[i])
	{
		update_quote_state(line[i], &state);
		if (state == 0)
		{
			if (check_invalid_operator(line, i))
				return (1);
			if ((line[i] == '<' || line[i] == '>') && line[i + 1] == line[i])
				i++; 
		}
		i++;
	}
	return (0);
}

//기본 체크 함수(parsing commnad에서 사용할 메인 함수)
void	check_quote(char **line, t_info_env *env)
{
	int	i;
	int	state;

	(void)env;
	while (1)
	{
		i = 0;
		state = 0;
		if (!line || !(*line))
			return ;
		while ((*line)[i])
			update_quote_state((*line)[i++], &state);
		if (state == 0)
			break ;
		if (!plus_line(line))
			return ;
	}
}
