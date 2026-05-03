/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:41:42 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 11:52:28 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "parsing.h"

static int	hex_val(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

char	*append_utf8(char *str, int cp)
{
	if (cp <= 0x7F)
		str = append_char(str, (char)cp);
	else if (cp <= 0x7FF)
	{
		str = append_char(str, (char)(0xC0 | (cp >> 6)));
		str = append_char(str, (char)(0x80 | (cp & 0x3F)));
	}
	else if (cp <= 0xFFFF)
	{
		str = append_char(str, (char)(0xE0 | (cp >> 12)));
		str = append_char(str, (char)(0x80 | ((cp >> 6) & 0x3F)));
		str = append_char(str, (char)(0x80 | (cp & 0x3F)));
	}
	else if (cp <= 0x10FFFF)
	{
		str = append_char(str, (char)(0xF0 | (cp >> 18)));
		str = append_char(str, (char)(0x80 | ((cp >> 12) & 0x3F)));
		str = append_char(str, (char)(0x80 | ((cp >> 6) & 0x3F)));
		str = append_char(str, (char)(0x80 | (cp & 0x3F)));
	}
	return (str);
}

static char	get_ansi_char(char c)
{
	if (c == 'n')
		return ('\n');
	if (c == 't')
		return ('\t');
	if (c == 'r')
		return ('\r');
	if (c == 'a')
		return ('\a');
	if (c == 'b')
		return ('\b');
	if (c == '\\')
		return ('\\');
	if (c == '\'')
		return ('\'');
	if (c == '\"')
		return ('\"');
	return (0);
}

static int	parse_hex_escape(char *line, int *i, int max_len)
{
	int	val;
	int	j;

	val = 0;
	j = 0;
	while (j < max_len && hex_val(line[*i + 1]) != -1)
	{
		val = (val << 4) | hex_val(line[++(*i)]);
		j++;
	}
	return (val);
}

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

static void	append_expanded_val(char **res, char *val, t_info_env *env, int state)
{
	char	*val_dup;
	char	*ifs;
	int		k;

	if (!val)
		return ;
	val_dup = ft_strdup(val);
	if (state == 0)
	{
		ifs = get_env_val_all(env, "IFS");
		if (!ifs)
			ifs = " \t\n";
		k = -1;
		while (val_dup[++k])
		{
			if (ft_strchr(ifs, val_dup[k]))
				val_dup[k] = '\x1F';
		}
	}
	*res = ft_strjoin_free(*res, val_dup);
	free(val_dup);
}

static int	handle_dollar(char **res, char *line, int i, t_info_env *env, int state)
{
	char	*name;
	char	*val;
	int		start;

	if (line[++i] == '?' || line[i] == '$')
	{
		if (line[i] == '?')
			val = ft_itoa(env->exit_code);
		else
			val = ft_itoa(getpid());
		append_expanded_val(res, val, env, state);
		free(val);
		return (i + 1);
	}
	if (ft_isalnum(line[i]) || line[i] == '_')
	{
		start = i;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		name = ft_substr(line, start, i - start);
		append_expanded_val(res, get_env_val_all(env, name), env, state);
		free(name);
		return (i);
	}
	*res = append_char(*res, '$');
	return (i);
}

int	handle_ansi_c_quoting(char **res, char *line, int i)
{
	char	c;

	*res = append_char(*res, '\'');
	i++;
	while (line[i] && line[i] != '\'')
	{
		if (line[i] == '\\' && line[i + 1])
		{
			c = get_ansi_char(line[++i]);
			if (c)
				*res = append_char(*res, c);
			else if (line[i] == 'x')
				*res = append_char(*res, parse_hex_escape(line, &i, 2));
			else if (line[i] == 'u')
				*res = append_utf8(*res, parse_hex_escape(line, &i, 4));
			else if (line[i] == 'U')
				*res = append_utf8(*res, parse_hex_escape(line, &i, 8));
			else
			{
				*res = append_char(*res, '\\');
				*res = append_char(*res, line[i]);
			}
		}
		else
			*res = append_char(*res, line[i]);
		i++;
	}
	*res = append_char(*res, '\'');
	if (line[i] == '\'')
		i++;
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
		if ((*line)[i] == '$' && (*line)[i + 1] == '\'' && !state)
		{
			i = handle_ansi_c_quoting(&res, *line, i + 1);
			continue ;
		}
		if ((*line)[i] == '$' && !(state & 1))
			i = handle_dollar(&res, *line, i, env, state);
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
