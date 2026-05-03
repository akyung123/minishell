/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:01:33 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:12:29 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "quote.h"

static int	get_padded_len(char *line)
{
	int	i;
	int	len;
	int	st;

	i = -1;
	len = 0;
	st = 0;
	while (line[++i])
	{
		if (line[i] == '\\' && line[i + 1] && st != 1 && ++i)
		{
			len += 2;
			continue ;
		}
		update_quote_state(line[i], &st);
		len++;
		if (!st && (line[i] == '<' || line[i] == '>'))
			len += 2 + (line[i + 1] == line[i]);
	}
	return (len);
}

static void	insert_redir_space(char *space, char *line, int *i, int *j)
{
	space[(*j)++] = ' ';
	space[(*j)++] = line[*i];
	if (line[*i + 1] == line[*i])
		space[(*j)++] = line[++(*i)];
	space[(*j)++] = ' ';
}

static void	fill_redir_space(char *space, char *line)
{
	int	i;
	int	j;
	int	st;

	i = -1;
	j = 0;
	st = 0;
	while (line[++i])
	{
		if (line[i] == '\\' && line[i + 1] && st != 1 && ++i)
		{
			space[j++] = '\\';
			space[j++] = line[i];
			continue ;
		}
		update_quote_state(line[i], &st);
		if (!st && (line[i] == '<' || line[i] == '>'))
			insert_redir_space(space, line, &i, &j);
		else
			space[j++] = line[i];
	}
	space[j] = '\0';
}

static char	*add_space_around_redir(char *line)
{
	char	*space;

	space = malloc(get_padded_len(line) + 1);
	if (!space)
		return (NULL);
	fill_redir_space(space, line);
	return (space);
}

char	**tokenize_line(char *line)
{
	char	**tokens;
	char	*padded_line;

	padded_line = add_space_around_redir(line);
	if (!padded_line)
		return (NULL);
	tokens = split_quotes(padded_line);
	free(padded_line);
	if (!tokens)
		return (NULL);
	return (tokens);
}
