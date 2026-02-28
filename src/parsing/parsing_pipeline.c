/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:57:54 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 13:04:31 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// p_op : pipe operator
t_pipeline	*parsing_pipeline(char *line)
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
	p_op = ft_strrchr(line, '|');
	if (p_op != NULL)
	{
		tmp = ft_substr(line, 0, p_op - line);
		pipeline->next = parsing_pipeline(tmp);
		free(tmp);
		tmp = ft_strdup(p_op + 1);
		pipeline->simple_command = parsing_simple_command(tmp);
		free(tmp);
	}
	else
		pipeline->simple_command = parsing_simple_command(line);
	return (pipeline);
}