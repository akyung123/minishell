/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/02/20 19:47:21 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "passing.h"

int	main(int argc, char **argv, char **envp)
{
	t_info_env	env;
	char *line;
	t_simple_command	*simple_command;

	(void)argc;
	(void)argv;
	init_env(&env, envp);
	while (1)
	{
		line = readline("minishell: ");
		// line을 passing하는 함수 필요함
		simple_command = passing_command(line);
		builin_handler(&env, simple_command);
	}
	all_free_env(env.head);
}
