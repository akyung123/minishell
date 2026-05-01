/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:57:54 by akkim             #+#    #+#             */
/*   Updated: 2026/04/30 04:53:31 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// a << 1 << 2 << 3 인경우 처리해야한다
// a << 1 | << 2 | << 3으로 보는 것이 좋음

// p_op : pipe operator
t_pipeline	*parsing_pipeline(t_info_env *env, char *line)
{
	t_pipeline	*pipeline;
	char		*tmp;
	char		*p_op;

	if (!line)
		return (NULL);
	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	pipeline->next = 0;
	p_op = ft_strchr(line, '|');
	pipeline->simple_command = ft_calloc(1, sizeof(t_simple_command));
	if (p_op != NULL)
	{
		tmp = ft_substr(line, 0, p_op - line);
		pipeline->simple_command->line = ft_strdup(tmp);
		free(tmp);
		tmp = ft_strdup(p_op + 1);
		pipeline->next = parsing_pipeline(env, tmp);
	}
	else
		pipeline->simple_command->line = ft_strdup(line);
	free(line);
	return (pipeline);
}
