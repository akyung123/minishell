/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 19:04:34 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 19:16:36 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

static int	is_valid_id(char *key)
{
	int	i;

	if (!key || !key[0] || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

// remove node for key and free node
void	mini_unset(t_info_env *env, char **args)
{
	int	i;

	if (!args || !args[1])
	{
		env->exit_code = 0;
		return ;
	}
	env->exit_code = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
			env->exit_code = 0;
		else
		{
			if (find_env_node(env->head, args[i]))
				remove_env(env, args[i]);
		}
		i++;
	}
}
