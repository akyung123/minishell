/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:34:25 by akkim             #+#    #+#             */
/*   Updated: 2026/04/30 06:39:02 by akkim            ###   ########.fr       */
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
	ft_memset(token, 0, 3);
	token[0] = line[idx];
	if (line[idx + 1] && line[idx] == line[idx + 1])
		token[1] = line[idx + 1];
	return (syntax_err_msg(token));
}
int get_op_len(char *line, int i)
{
    char c = line[i];

    if ((c == '|' || c == '&' || c == '<' || c == '>') && line[i + 1] == c)
        return (2);
    return (1);
}

int	check_invalid_operator(char *line, int i)
{
	int     next;
    int op_len;

    op_len = get_op_len(line, i);
    // 1. 현재 연산자의 길이를 확정 (&&, ||, <<, >> 인지 확인)
	if (line[i] == '&' && op_len == 1)
        return (print_unexpected_token(line, i));
	next = skip_spaces(line, i + op_len);
	if (line[next] == '\0' || line[next] == '<' || line[next] == '&' || line[next] == '>' || line[next] == '|')
			return (print_unexpected_token(line, next));
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
		if (line[i] == '\\' && line[i + 1] && state != 1)
			i += 2;
		else
			update_quote_state(line[i++], &state);
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
	if (line[i] == '|' || line[i] == '&') // '|||'나 '||||'이 들어오는 경우 처리해야함
		return (print_unexpected_token(line, i));
	state = 0;
	while (line[i])
	{
		update_quote_state(line[i], &state);
		if (state == 0)
		{
			if (line[i] == '|' || line[i] == '&' || line[i] == '<' || line[i] == '>')
			{	
				if (check_invalid_operator(line, i))
					return (1);
				i += get_op_len(line, i) - 1;
			}
		}
		i++;
	}
	return (0);
}
