/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:34:25 by akkim             #+#    #+#             */
/*   Updated: 2026/04/29 13:55:09 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

int	skip_spaces(char *line, int i)
{
	while (line[i] && line[i] == ' ')
		i++;
	return (i);
}

int	print_unexpected_token(char *line, int idx)
{
	char	token[3];

	if (line[idx] == '\0')
		return (syntax_err_msg("newline"));
	token[0] = line[idx];
	token[1] = '\0';
	token[2] = '\0';
	if ((line[idx] == '<' || line[idx] == '>') && line[idx] == line[idx + 1])
		token[1] = line[idx + 1];
	return (syntax_err_msg(token));
}

int	check_invalid_operator(char *line, int i)
{
	int	next;

	if (line[i] == '|')
	{
		if (line[i + 1] == '|')
			return (0);
		next = skip_spaces(line, i + 1);
		if (line[next] == '|' || line[next] == '\0')
			return (print_unexpected_token(line, next));
	}
	else if (line[i] == '<' || line[i] == '>')
	{
		if (line[i + 1] == line[i])
			i++;
		next = skip_spaces(line, i + 1);
		if (line[next] == '\0' || line[next] == '<'
			|| line[next] == '>' || line[next] == '|')
			return (print_unexpected_token(line, next));
	}
	return (0);
}

int	check_unclosed_quotes(char *line)
{
	int	i;
	int	state;

	i = 0;
	state = 0;
	while (line[i])
	{
		update_quote_state(line[i], &state);
		i++;
	}
	if (state != 0)
		return (1);
	return (0);
}

int	check_syntax_error(char *line)
{
	int	i;
	int	state;

	if (check_unclosed_quotes(line))
		return (syntax_err_msg("unclosed quotes"));
	i = skip_spaces(line, 0);
	if (line[i] == '|')
		return (print_unexpected_token(line, i));
	state = 0;
	while (line[i])
	{
		update_quote_state(line[i], &state);
		if (state == 0)
		{
			if (check_invalid_operator(line, i))
				return (1);
			if ((line[i] == '<' || line[i] == '>') && line[i + 1] == line[i])
				i++;
		}
		i++;
	}
	return (0);
}
