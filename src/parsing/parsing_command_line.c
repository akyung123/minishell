/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:19 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 08:36:48 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "parsing.h"
#include "minishell.h"

// 현재 인덱스가 && 또는 || 인지 확인하는 함수
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
	char	*tmp;

	cmd->comm_oper = comm[0];
	tmp = ft_substr(line, 0, comm - line);
	cmd->pipeline = parsing_pipeline(env, tmp);
	free(tmp);
	tmp = ft_strdup(comm + 2);
	cmd->next = parsing_command_line(env, &tmp);
	free(tmp);
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
