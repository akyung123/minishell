/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 18:59:32 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 06:10:32 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_env(t_info_env *env)
{
	t_env	*e;

	e = env->head;
	while (e->value)
	{
		ft_printf("%s=%s\n", e->key, e->value);
		e = e->next;
	}
	env->exit_code = 0;
}
