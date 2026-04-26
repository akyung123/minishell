/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:34:25 by akkim             #+#    #+#             */
/*   Updated: 2026/04/26 18:58:41 by akkim            ###   ########.fr       */
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
			return ;	// error
	}
	// // ""제거 및 변환 함수
	// refine_line(line, env);
}