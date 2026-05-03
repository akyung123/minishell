/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:23:18 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 14:39:29 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "parsing.h"

char	*append_utf8(char *str, int cp)
{
	if (cp <= 0x7F)
		str = append_char(str, (char)cp);
	else if (cp <= 0x7FF)
	{
		str = append_char(str, (char)(0xC0 | (cp >> 6)));
		str = append_char(str, (char)(0x80 | (cp & 0x3F)));
	}
	else if (cp <= 0xFFFF)
	{
		str = append_char(str, (char)(0xE0 | (cp >> 12)));
		str = append_char(str, (char)(0x80 | ((cp >> 6) & 0x3F)));
		str = append_char(str, (char)(0x80 | (cp & 0x3F)));
	}
	else if (cp <= 0x10FFFF)
	{
		str = append_char(str, (char)(0xF0 | (cp >> 18)));
		str = append_char(str, (char)(0x80 | ((cp >> 12) & 0x3F)));
		str = append_char(str, (char)(0x80 | ((cp >> 6) & 0x3F)));
		str = append_char(str, (char)(0x80 | (cp & 0x3F)));
	}
	return (str);
}

void	handle_esc_fallback(char **res, char c)
{
	if (!res)
		return ;
	*res = append_char(*res, '\\');
	*res = append_char(*res, c);
}

static char	get_ansi_char(char c)
{
	if (c == 'n')
		return ('\n');
	if (c == 't')
		return ('\t');
	if (c == 'r')
		return ('\r');
	if (c == 'a')
		return ('\a');
	if (c == 'b')
		return ('\b');
	if (c == '\\')
		return ('\\');
	if (c == '\'')
		return ('\'');
	if (c == '\"')
		return ('\"');
	return (0);
}

int	handle_ansi_c_quoting(char **res, char *line, int i)
{
	char	c;

	*res = append_char(*res, '\'');
	while (line[++i] && line[i] != '\'')
	{
		if (line[i] == '\\' && line[i + 1] && ++i)
		{
			c = get_ansi_char(line[i]);
			if (c)
				*res = append_char(*res, c);
			else if (line[i] == 'x')
				*res = append_char(*res, parse_hex_escape(line, &i, 2));
			else if (line[i] == 'u')
				*res = append_utf8(*res, parse_hex_escape(line, &i, 4));
			else if (line[i] == 'U')
				*res = append_utf8(*res, parse_hex_escape(line, &i, 8));
			else
				handle_esc_fallback(res, line[i]);
		}
		else
			*res = append_char(*res, line[i]);
	}
	*res = append_char(*res, '\'');
	return (i + (line[i] == '\''));
}
