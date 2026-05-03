/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasring_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:11 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:12:50 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_redir(char *token)
{
	if (!ft_strcmp(token, ">") || !ft_strcmp(token, "<")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"))
		return (1);
	return (0);
}

int	check_syntax(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens[0])
		return (0);
	while (tokens[i])
	{
		if (is_redir(tokens[i]))
		{
			if (tokens[i + 1] == NULL)
				return (0);
			else if (is_redir(tokens[i + 1]))
				return (0);
			i += 2;
		}
		else
			i++;
	}
	return (1);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

// <simple_command> ::= <cmd_name> <arguments_opt> <redirects_opt>
t_simple_command	*parsing_simple_command(t_info_env *env, char *line)
{
	t_simple_command	*simple_command;
	char				**tokens;

	(void)env;
	if (!line)
		return (NULL);
	tokens = tokenize_line(line);
	if (!tokens)
		return (NULL);
	if (!check_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	simple_command = build_cmd_struct(tokens, env);
	free_tokens(tokens);
	return (simple_command);
}
