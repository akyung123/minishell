/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:46:50 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 03:14:11 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "quote.h"

void	add_expanded_args(t_simple_command *cmd, char *token, int *j)
{
	char	**split;
	char	**new_args;
	int		k;
	int		len;

	split = ft_split(token, ' ');
	len = 0;
	while (split && split[len])
		len++;
	new_args = ft_calloc(*j + len + 1, sizeof(char *));
	k = -1;
	while (++k < *j)
		new_args[k] = cmd->args[k];
	k = -1;
	while (++k < len)
	{
		if (*j == 0 && k == 0)
			cmd->cmd = ft_strdup(split[k]);
		new_args[*j + k] = ft_strdup(split[k]);
	}
	free(cmd->args);
	cmd->args = new_args;
	*j += len;
	free_tokens(split);
}

static char	*get_next_word(char *s, int *i)
{
	int	start;
	int	state;

	state = 0;
	while (s[*i] == ' ')
		(*i)++;
	start = *i;
	while (s[*i])
	{
		update_quote_state(s[*i], &state);
		if (s[*i] == ' ' && !state)
			break ;
		(*i)++;
	}
	return (ft_substr(s, start, *i - start));
}

static int	count_words(char *s)
{
	int	count;
	int	i;
	int	state;

	count = 0;
	i = 0;
	state = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (s[i])
			count++;
		while (s[i])
		{
			update_quote_state(s[i], &state);
			if (s[i] == ' ' && !state)
				break ;
			i++;
		}
	}
	return (count);
}

// 공백기준으로 쪼개는 함수
// but, ft_split과 다르게 "" 안의 공백은 무시해야함
char	**split_quotes(char *s)
{
	char	**split;
	int		i;
	int		j;
	int		words;

	words = count_words(s);
	split = malloc(sizeof(char *) * (words + 1));
	if (!split)
		return (NULL);
	i = 0;
	j = 0;
	while (j < words)
	{
		split[j] = get_next_word(s, &i);
		j++;
	}
	split[j] = NULL;
	return (split);
}
