/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:27:34 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 16:05:40 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "quote.h"

char	*find_unquoted_pipe(char *line)
{
	int	i;
	int	st;

	i = -1;
	st = 0;
	while (line[++i])
	{
		if (line[i] == '\\' && line[i + 1] && st != 1 && ++i)
			continue ;
		update_quote_state(line[i], &st);
		if (st == 0 && line[i] == '|')
			return (&line[i]);
	}
	return (NULL);
}

char	*minishell_strrchr(const char *str, int ch)
{
	char	*last;
	int		i;

	last = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == (unsigned char)ch)
		{
			last = (char *)&str[i];
		}
		i++;
	}
	if ((unsigned char)ch == '\0')
	{
		return ((char *)&str[i]);
	}
	return (last);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}
