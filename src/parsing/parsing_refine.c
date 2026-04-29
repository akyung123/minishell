/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:41:42 by akkim             #+#    #+#             */
/*   Updated: 2026/04/29 13:46:32 by akkim            ###   ########.fr       */
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

static int	handle_dollar(char **res, char *line, int i, t_info_env *env)
{
	char	*name;
	char	*val;
	int		start;

	if (line[++i] == '?')
	{
		val = ft_itoa(env->exit_code);
		*res = ft_strjoin_free(*res, val);
		free(val);
		return (i + 1);
	}
	if (line[i] == '$')
	{
		val = ft_itoa(getpid());
		*res = ft_strjoin_free(*res, val);
		free(val);
		return (i + 1);
	}
	if (ft_isalnum(line[i]) || line[i] == '_')
	{
		start = i;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		name = ft_substr(line, start, i - start);
		val = get_env_val(env->head, name);
		if (val)
			*res = ft_strjoin_free(*res, val);
		free(name);
		return (i);
	}
	*res = append_char(*res, '$');
	return (i);
}

void	expand_env(char **line, t_info_env *env)
{
	char	*res;
	int		i;
	int		state;

	if (!line || !(*line))
		return ;
	res = ft_strdup("");
	i = 0;
	state = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '\\' && (*line)[i + 1])
		{
			res = append_char(res, (*line)[i++]);
			res = append_char(res, (*line)[i++]);
			continue ;
		}
		update_quote_state((*line)[i], &state);
		if ((*line)[i] == '$' && !(state & 1))
			i = handle_dollar(&res, *line, i, env);
		else
			res = append_char(res, (*line)[i++]);
	}
	free(*line);
	*line = res;
}

// 따옴표 제거 함수
void	remove_quotes(char *line)
{
	int		i;
	int		j;
	int		state;
	int		prev_state;

	if (!line)
		return ;
	i = 0;
	j = 0;
	state = 0;
	while (line[i])
	{
		if (line[i] == '\\' && line[i + 1 ] && state != 1)
		{
			i++; 
			line[j++] = line[i++];
			continue ;
		}
		prev_state = state;
		update_quote_state(line[i], &state);
		if (state != prev_state)
		{
			i++;
			continue ;
		}
		line[j++] = line[i++];
	}
	line[j] = '\0';
}

void	refine_line(char **line, t_info_env *env)
{
	if (!line || !(*line))
		return ;
	process_backticks(line, env);
	expand_env(line, env);
	if (*line)
	{
		remove_quotes(*line);
		restore_expanded_quotes(*line);
	}
}
