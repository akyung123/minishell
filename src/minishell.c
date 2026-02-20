/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/02/20 16:36:46 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

int	main(int argc, char **argv, char **envp)
{
	t_info_env	env;
	char *code[5] = {NULL, };

	(void)argc;
	(void)argv;
	init_env(&env, envp);
	code[0] = ft_strdup("sdfs");
	code[1] = ft_strdup("sdfs");
	mini_exit(&env, code);
	all_free_env(env.head);
}
