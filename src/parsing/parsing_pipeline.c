/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:57:54 by akkim             #+#    #+#             */
/*   Updated: 2026/04/25 11:42:02 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
	if (p_op != NULL)
	{
		tmp = ft_substr(line, 0, p_op - line);
		pipeline->simple_command = parsing_simple_command(env, tmp);
		free(tmp);
		tmp = ft_strdup(p_op + 1);
		pipeline->next = parsing_pipeline(env, tmp);
		free(tmp);
	}
	else
		pipeline->simple_command = parsing_simple_command(env, line);
	return (pipeline);
}