/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 18:59:32 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 02:03:39 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_env(t_info_env *env)
{
	t_env	*e;

	e = env->head;
	while (e->next)
	{
		ft_printf("%s=%s\n", e->key, e->value);
		e = e->next;
	}
	env->exit_code = 0;
}
