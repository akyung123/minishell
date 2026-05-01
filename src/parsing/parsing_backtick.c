/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_backtick.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 00:58:26 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 01:14:39 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
#include "execution.h"
#include "quote.h"

static int	handle_backtick(t_parse_info *info, t_info_env *env)
{
	char	*inner;
	char	*out;
	int		start;

	start = ++(info->i);
	while (info->line[info->i] && info->line[info->i] != '`')
		(info->i)++;
	inner = ft_substr(info->line, start, info->i - start);
	ft_printf("inner : %s\n", inner);
	out = execute_backtick(inner, env);
	ft_printf("out : %s\n", out);
	free(inner);
	if (out)
	{
		*(info->res) = ft_strjoin_free(*(info->res), out);
		//free(out);
	}
	if (info->line[info->i] == '`')
	{
		update_quote_state(info->line[info->i], info->state);
		(info->i)++;
	}
	return (info->i);
}

void	process_backticks(char **line, t_info_env *env)
{
	char			*res;
	int				i;
	int				state;
	t_parse_info	info;

	if (!line || !*line)
		return ;
	res = ft_strdup("");
	i = 0;
	state = 0;
	info.res = &res;
	info.line = *line;
	info.state = &state;
	while ((*line)[i])
	{
		info.i = i;
		// 굳이 필요한가?
		update_quote_state((*line)[i], &state);
		if ((*line)[i] == '`' && !(state & 1))
			i = handle_backtick(&info, env);
		else
			res = append_char(res, (*line)[i++]);
	}
	free(*line);
	*line = res;
}
