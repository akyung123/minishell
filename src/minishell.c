/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 02:32:12 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "parsing.h"

int	g_signo;

int	is_only_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_line(t_info_env *env, char *line)
{
	t_command_line	*cmd;

	if (g_signo != 0)
	{
		env->exit_code = g_signo;
		g_signo = 0;
	}
	if (is_only_whitespace(line))
		return (0);
	if (*line)
		add_history(line);
	cmd = parsing_command_line(env, &line);
	if (cmd)
		executor_command_line(env, cmd);
	free_command_line(cmd);
	return (1);
}

void	shell_loop(t_info_env *env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell: ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (!handle_line(env, line))
		{
			free(line);
			continue ;
		}
		free(line);
	}
}

void	shell_cleanup(t_info_env *env)
{
	all_free_env(env->head);
}

int	main(int argc, char **argv, char **envp)
{
	t_info_env	env;

	(void)argc;
	(void)argv;
	init_env(&env, envp);
	set_terminal_print_off();
	set_signal();
	shell_loop(&env);
	shell_cleanup(&env);
	return (0);
}
