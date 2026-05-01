/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 02:17:32 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "parsing.h"
#include "quote.h"

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
	t_command_line	*command_line;
	char			*working_line;

	if (g_signo != 0)
	{
		env->exit_code = g_signo;
		g_signo = 0;
	}
	if (is_only_whitespace(line))
		return (0);
	working_line = ft_strdup(line);
	check_quote(&working_line, env);
	if (!working_line)
		return (1);
	command_line = parsing_command_line(env, &working_line);
	if (command_line)
		executor_command_line(env, command_line);
	else
		return (0);
	free_command_line(command_line);
	free(working_line);
	return (1);
}

void	shell_loop(t_info_env *env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell: ");
		if (line)
			add_history(line);
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
	all_free_env(env->hide_head);
	if (env->envp)
		free_split(env->envp);
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
	rl_clear_history();
	return (0);
}
