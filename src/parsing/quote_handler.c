/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:23:26 by akkim             #+#    #+#             */
/*   Updated: 2026/03/16 15:56:07 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

// 입력받아서 합치는 함수
void	plus_line(char **line)
{
	char	*new;
	char	*tmp;

	new = readline("> ");
	tmp = ft_strjoin(*line, new);
	free(*line);
	free(new);
	*line = tmp;
}

int	skip_quote(char **line, int start_i, char type)
{
	int	i;

	i = start_i + 1;
	while (1)
	{
		if ((*line)[i] == '\0')
			plus_line(line);
		else if ((type == '\"' && (*line)[i] == '`'))
			i = skip_quote(line, i, '`') + 1;
		else if ((*line)[i] == type)
			return (i);
		else
			i++;
	}
}
