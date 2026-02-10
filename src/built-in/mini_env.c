/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 18:59:32 by akkim             #+#    #+#             */
/*   Updated: 2026/02/07 19:55:52 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// 그냥 envp 출력시키면 되려나

void	mini_env(t_info_env *env)
{
	int		i;
	t_env	*e;

	i = 0;
	e = env->head;
	while (e->next)
	{
		ft_printf("%s=%s\n", e->key, e->value);
		e = e->next;
	}
	// ft_printf("_=/usr/bin/env\n");
}
