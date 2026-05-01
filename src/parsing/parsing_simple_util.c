/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:01:33 by akkim             #+#    #+#             */
/*   Updated: 2026/04/29 15:21:30 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "quote.h"

static int	get_padded_len(char *line)
{
	int	i;
	int	len;
	int	state;

	i = 0;
	len = 0;
	state = 0;
	while (line[i])
	{
		if (line[i] == '\\' && line[i + 1] && state != 1)
		{
			len += 2;
			i += 2;
			continue ;
		}
		update_quote_state(line[i], &state);
		if (!state && (line[i] == '<' || line[i] == '>'))
		{
			len += 2;
			if (line[i + 1] == line[i])
				len ++;
			len++;
		}
		else
			len++;
		i++;
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

// 공백 추가 함수
// 리디렉션 앞뒤에 공백이 없을 경우,  추가하여 split을 원활하게 돕는다.
static char	*add_space_around_redir(char *line)
{
	char		*space;
	int			state;
	int			i;
	int			j;

	i = -1;
	j = 0;
	state = 0;
	space = malloc(sizeof(char) * (get_padded_len(line)) + 1);
	if (!space)
		return (NULL);
	while (line[++i])
	{
		if (line[i] == '\\' && line[i + 1] && state != 1)
		{
			space[j++] = line[i++];
			space[j++] = line[i];
			continue ;
		}
		update_quote_state(line[i], &state);
		if (!state && (line[i] == '<' || line[i] == '>'))
			insert_redir_space(space, line, &i, &j);
		else
			space[j++] = line[i];
	}
	space[j] = '\0';
	return (space);
}

// 토큰화 함수
// 내부에서 add_space_around_redir 후 ft_split 실행
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
