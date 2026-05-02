/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:36:56 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 18:33:18 by akkim            ###   ########.fr       */
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
static int	plus_line(char **line, t_info_env *env, int state)
{
	char	*new;
	char	*tmp1;
	char	*tmp2;

	// if (isatty(STDIN_FILENO))
	// 	write(1, "> ", 2);
	// new = get_next_line(0);
	new = readline("> ");
	(void)env;
	if (!new)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching ", 2);
		if (state & 1)
			ft_putstr_fd("`''\n", 2);
		else if (state & 2)
			ft_putstr_fd("`\"'\n", 2);
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
		free(*line);
		*line = NULL;
		exit(2);
	}
	tmp1 = ft_strjoin(*line, "\n");
	if (!tmp1)
	{
		free(new);
		return (0);
	}
	tmp2 = ft_strjoin(tmp1, new);
	free(tmp1);
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
		{
			if ((*line)[i] == '\\' && (*line)[i + 1])
			{
				i += 2;
				continue ;
			}
			update_quote_state((*line)[i++], &state);
		}
		if (state == 0)
			break ;
		if (!plus_line(line, env, state))
			return ;
	}
}
