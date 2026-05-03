/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:41:42 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 13:10:36 by akkim            ###   ########.fr       */
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

static int	handle_dollar(char **res, char *str, t_info_env *env, int st)
{
	char	*name;
	char	*val;
	int		len;

	if (str[1] == '?' || str[1] == '$')
	{
		val = (str[1] == '?') ? ft_itoa(env->exit_code) : ft_itoa(getpid());
		append_expanded_val(res, val, env, st);
		return (free(val), 2);
	}
	if (ft_isalnum(str[1]) || str[1] == '_')
	{
		len = 1;
		while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
			len++;
		name = ft_substr(str, 1, len - 1);
		append_expanded_val(res, get_env_val_all(env, name), env, st);
		return (free(name), len);
	}
	*res = append_char(*res, '$');
	return (1);
}

void	handle_esc_fallback(char **res, char c)
{
	if (!res)
		return ;
	*res = append_char(*res, '\\');
	*res = append_char(*res, c);
}

int	handle_ansi_c_quoting(char **res, char *line, int i)
{
	char	c;

	*res = append_char(*res, '\'');
	while (line[++i] && line[i] != '\'')
	{
		if (line[i] == '\\' && line[i + 1] && ++i)
		{
			c = get_ansi_char(line[i]);
			if (c)
				*res = append_char(*res, c);
			else if (line[i] == 'x')
				*res = append_char(*res, parse_hex_escape(line, &i, 2));
			else if (line[i] == 'u')
				*res = append_utf8(*res, parse_hex_escape(line, &i, 4));
			else if (line[i] == 'U')
				*res = append_utf8(*res, parse_hex_escape(line, &i, 8));
			else
				handle_esc_fallback(res, line[i]);
		}
		else
			*res = append_char(*res, line[i]);
	}
	*res = append_char(*res, '\'');
	return (i + (line[i] == '\''));
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
		if (line[i] == '\\' && line[i + 1 ] && state != 1)
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
