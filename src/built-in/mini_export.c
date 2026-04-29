/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:13:55 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 02:18:22 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

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
static void	set_env_value(t_info_env *env, char *key, char *value)
{
	t_env	*node;

	node = find_env_node(env->head, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		node->value = ft_strdup(value);
	}
	else
		env_add_back(&env->head, new_env_node(key, value));
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
static void	handle_equal(t_info_env *env, char *str, char *eq)
{
	char	*key;
	char	*value;

	key = ft_substr(str, 0, eq - str);
	if (!key || !valid_identifier(key))
	{
		export_error(env, str);
		free(key);
		env->exit_code = 1;
		return ;
	}
	value = ft_strdup(eq + 1);
	if (!value)
	{
		free(key);
		env->exit_code = 1;
		return ;
	}
	set_env_value(env, key, value);
	free(key);
	free(value);
	env->exit_code = 0;
}

void	mini_export(t_info_env *env, char *str)
{
	char	*eq;

	if (!str)
	{
		print_sorted_export(env);
		env->exit_code = 0;
		return ;
	}
	eq = ft_strchr(str, '=');
	if (!eq)
		return (handle_no_equal(env, str));
	handle_equal(env, str, eq);
}
