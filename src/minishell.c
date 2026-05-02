/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 23:04:08 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "parsing.h"
#include "quote.h"

int	g_signo;

void	shell_cleanup(t_info_env *env)
{
	all_free_env(env->head);
	all_free_env(env->hide_head);
	if (env->envp)
		free_split(env->envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_info_env	env;
	int			exit_code;

	(void)argc;
	(void)argv;
	init_env(&env, envp);
	set_terminal_print_off();
	set_signal();
	shell_loop(&env);
	exit_code = env.exit_code;
	shell_cleanup(&env);
	rl_clear_history();
	return (exit_code);
}
