/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:36:56 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 03:03:11 by akkim            ###   ########.fr       */
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
static int	plus_line(char **line)
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
