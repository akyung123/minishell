/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:23:26 by akkim             #+#    #+#             */
/*   Updated: 2026/03/23 02:25:05 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"


// 입력받아서 합치는 함수
int	plus_line(char **line)
{
	char	*new;
	char	*tmp;

	new = readline("> ");
	if (!new)
		return (0);
	tmp = ft_strjoin(*line, new);
	if (!tmp)
	{
		free(new);
		return (0);
	}
	free(*line);
	free(new);
	*line = tmp;
	return (1);
}

int	skip_quote(char **line, int start_i, char type)
{
	int	i;
	int	res;

	i = start_i + 1;
	while (1)
	{
		if (!*line)
			return (-1);
		if ((*line)[i] == '\0')
		{
			if (!plus_line(line))
				return (-1);
			i = 0;
		}
		else if ((type == '\"' && (*line)[i] == '`'))
		{
			res = skip_quote(line, i, '`');
			if (res == -1)
				return (-1);
			i = res + 1;
		}
		else if ((*line)[i] == type)
			return (i);
		else
			i++;
	}
}
