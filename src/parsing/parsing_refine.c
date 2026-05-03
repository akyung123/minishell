/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:41:42 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 14:38:20 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "parsing.h"

char	*append_char(char *str, char c)
{
	char	*new_str;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	ft_strlcpy(new_str, str, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

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

// 따옴표 제거 함수
void	remove_quotes(char *line)
{
	int		i;
	int		j;
	int		state;
	int		prev;

	if (!line)
		return ;
	i = 0;
	j = 0;
	state = 0;
	while (line[i])
	{
		if (line[i] == '\\' && line[i + 1] && state != 1)
		{
			prev = state;
			update_quote_state(line[i], &state);
			if (line[i] == '\\' && line[i + 1] && state != 1 && ++i)
				line[j++] = line[i++];
			else if (state == prev)
				line[j++] = line[i++];
			else
				i++;
		}
	}
	line[j] = '\0';
}

void	expand_env(char **line, t_info_env *env)
{
	char	*res;
	int		i;
	int		state;

	res = ft_strdup("");
	i = 0;
	state = 0;
	while (line && *line && (*line)[i])
	{
		update_quote_state((*line)[i], &state);
		if ((*line)[i] == '\\' && (*line)[i + 1] && state != 1)
		{
			res = append_char(res, (*line)[i++]);
			res = append_char(res, (*line)[i++]);
		}
		else if ((*line)[i] == '$' && (*line)[i + 1] == '\'' && !state)
			i = handle_ansi_c_quoting(&res, *line, i + 1);
		else if ((*line)[i] == '$' && !(state & 1))
			i += handle_dollar(&res, *line + i, env, state);
		else
			res = append_char(res, (*line)[i++]);
	}
	free(*line);
	*line = res;
}

// process_backticks(line, env);
// remove before eval
void	refine_line(char **line, t_info_env *env)
{
	if (!line || !(*line))
		return ;
	expand_env(line, env);
	if (*line)
		restore_expanded_quotes(*line);
}
