/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine_util2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:36:45 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:46:15 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "parsing.h"

static void	append_expanded_val(char **res, char *val,
	t_info_env *env, int state)
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

int	handle_dollar(char **res, char *str, t_info_env *env, int st)
{
	char	*name;
	int		len;

	if (str[1] == '?' || str[1] == '$')
	{
		if (str[1] == '?')
			name = ft_itoa(env->exit_code);
		else
			name = ft_itoa(getpid());
		append_expanded_val(res, name, env, st);
		return (free(name), 2);
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
	if (str[1] == '\"' && st == 0)
		return (1);
	*res = append_char(*res, '$');
	return (1);
}

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

int	parse_hex_escape(char *line, int *i, int max_len)
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
