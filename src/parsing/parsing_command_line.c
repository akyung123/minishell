/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:19 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:11:33 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

int	is_logical_operator(char *line, int i)
{
	if (line[i] == '&' && line[i + 1] == '&')
		return (1);
	if (line[i] == '|' && line[i + 1] == '|')
		return (1);
	return (0);
}

// find comm operator
char	*find_next_operator(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (is_logical_operator(line, i))
			return (&line[i]);
		else if (line[i])
			i++;
	}
	return (NULL);
}

static void	handle_operator(t_command_line *cmd, char *line,
	char *comm, t_info_env *env)
{
	char	*left_str;
	char	*right_str;
	char	*right_base;

	cmd->comm_oper = comm[0];
	left_str = ft_substr(line, 0, comm - line);
	cmd->pipeline = parsing_pipeline(env, left_str);
	right_str = ft_strdup(comm + 2);
	right_base = right_str;
	cmd->next = parsing_command_line(env, &right_str);
	free(right_base);
}

t_command_line	*parsing_command_line(t_info_env *env, char **line)
{
	t_command_line	*cmd;
	char			*comm;

	if (!line || !*line)
		return (NULL);
	if (check_syntax_error(*line))
	{
		env->exit_code = 2;
		return (NULL);
	}
	cmd = malloc(sizeof(t_command_line));
	if (!cmd)
		return (NULL);
	cmd->next = NULL;
	cmd->comm_oper = 0;
	comm = find_next_operator(*line);
	if (comm != NULL)
		handle_operator(cmd, *line, comm, env);
	else
		cmd->pipeline = parsing_pipeline(env, ft_strdup(*line));
	return (cmd);
}
