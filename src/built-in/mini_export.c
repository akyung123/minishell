/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:13:55 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 03:21:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
// 여러 개 인자를 받아도 가능해야함.

static int	valid_identifier(char *key)
{
	int	i;

	if (!key || !key[0])
		return (0);
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i ++;
	}
	return (1);
}

// set KEY = VALUE
static void	set_env_value(t_info_env *env, char *key, char *value, int append)
{
	t_env	*node;
	char	*temp;

	node = find_env_node(env->head, key);
	if (node)
	{
		if (append && node->value)
		{
			temp = ft_strjoin(node->value, value);
			free(node->value);
			node->value = temp;
		}
		else
		{
			if (node->value)
				free(node->value);
			node->value = ft_strdup(value);
		}
	}
	else
		env_add_back(&env->head, new_env_node(key, value));
}

static char	*trim_quotes(char *str)
{
	int		len;
	char	*new_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\"' && str[len - 1] == '\"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		new_str = ft_substr(str, 1, len - 2);
		return (new_str);
	}
	return (ft_strdup(str));
}

// Only KEY
static void	handle_no_equal(t_info_env *env, char *str)
{
	if (!valid_identifier(str))
	{
		export_error(env, str);
		env->exit_code = 1;
		return ;
	}
	if (!find_env_node(env->head, str))
		env_add_back(&env->head, new_env_node(str, NULL));
	env->exit_code = 0;
}

// KEY=VALUE
static void	handle_equal(t_info_env *env, char *str, char *eq, int append)
{
	char	*key;
	char	*value;
	char	*trimmed_value;

	key = ft_substr(str, 0, (eq - str) - append);
	if (!key || !valid_identifier(key))
	{
		export_error(env, str);
		free(key);
		env->exit_code = 1;
		return ;
	}
	value = ft_strdup(eq + 1);
	trimmed_value = trim_quotes(value);
	if (!value)
	{
		free(key);
		env->exit_code = 1;
		return ;
	}
	set_env_value(env, key, trimmed_value, append);
	free(key);
	free(value);
	free(trimmed_value);
	env->exit_code = 0;
}

void	mini_export(t_info_env *env, char **args)
{
	char	*eq;
	int		i;
	int		append;

	if (!args || !args[1])
	{
		print_sorted_export(env);
		env->exit_code = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		append = 0;
		eq = ft_strchr(args[i], '=');
		if (!eq)
			handle_no_equal(env, args[i]);
		else
		{
			if (eq > args[i] && *(eq - 1) == '+')
				append = 1;
			handle_equal(env, args[i], eq, append);
		}
		i++;
	}
}
