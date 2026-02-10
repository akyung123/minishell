/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:13:55 by akkim             #+#    #+#             */
/*   Updated: 2026/02/10 19:08:34 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

// Check if the first character is an alphabet or '_'
static int	chk_key(char *key)
{
	if (ft_isalpha(key[0]) || key[0] == '_')
		return (1);
	return (0);
}

void	mini_export(t_info_env *env, char *str)
{
	t_env	*node;
	char	*eq;
	char	*key;
	char	*value;

	if (!str)
		return ;
	eq = ft_strchr(str, '=');
	key = ft_substr(str, 0, eq - str);
	if (!chk_key(key))
		return ;
	value = ft_strdup(eq + 1);
	node = find_env_node(env->head, key);
	if (node != NULL)
		node->value = value;
	else
	{
		node = new_env_node(key, value);
		env_add_back(&env->head, node);
	}
}
