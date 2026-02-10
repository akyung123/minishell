/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 19:04:34 by akkim             #+#    #+#             */
/*   Updated: 2026/02/10 20:14:18 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

static int	chk_key(t_info_env *env, char *key)
{
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		env->exit_code = 1;
		return (1);
	}
	return (0);
}

// remove node for key and free node
void	mini_unset(t_info_env *env, char *key)
{
	t_env	*node;

	if (!key)
	{
		env->exit_code = 0;
		return ;
	}
	if (!chk_key(env, key))
		return ;
	node = find_env_node(env->head, key);
	if (node != NULL)
		remove_env(env, key);
	return ;
}
