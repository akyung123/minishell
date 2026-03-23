/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/03/23 16:48:30 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "parsing.h"
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	t_info_env		env;
	char			*line;
	t_command_line	*command_line;

	(void)argc;
	(void)argv;
	init_env(&env, envp);
	while (1)
	{
		line = readline("minishell: ");
		command_line = parsing_command_line(&line);
		test_command_line(command_line);
	}
	all_free_env(env.head);
}
