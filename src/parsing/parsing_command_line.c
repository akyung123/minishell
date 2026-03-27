/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:19 by akkim             #+#    #+#             */
/*   Updated: 2026/03/27 16:02:20 by akkim            ###   ########.fr       */
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

t_command_line	*parsing_command_line(t_info_env *env, char **line)
{
	t_command_line	*command_line;
	char			*tmp;
	char			*comm;

	if (!line)
		return (NULL);
	command_line = malloc(sizeof(t_command_line));
	if (!command_line)
		return (NULL);
	command_line->next = 0;
	command_line->comm_oper = 0;
	check_quote(line);
	comm = find_next_operator(*line);
	if (comm != 0)
	{
		command_line->comm_oper = comm[0];
		tmp = ft_substr(*line, 0, comm - (*line));
		command_line->pipeline = parsing_pipeline(env, tmp);
		free(tmp);
		tmp = ft_strdup(comm + 2);
		command_line->next = parsing_command_line(env, &tmp);
		free(tmp);
	}
	else
		command_line->pipeline = parsing_pipeline(env, ft_strdup(*line));
	return (command_line);
}
