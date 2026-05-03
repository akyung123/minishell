/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:01:37 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 17:00:08 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"
#include "quote.h"

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

	if (!line)
		return (0);
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

char	*read_line(t_info_env *env, char *s)
{
	char	*line;
	char	*trim_line;

	(void)env;
	if (isatty(STDIN_FILENO))
		line = readline(s);
	else
	{
		line = get_next_line(0);
		trim_line = ft_strtrim(line, "\n");
		free(line);
		line = trim_line;
	}
	return (line);
}

void	shell_loop(t_info_env *env)
{
	char	*line;

	while (1)
	{
		line = read_line(env, "minishell: ");
		if (line)
			add_history(line);
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				ft_printf("exit\n");
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
