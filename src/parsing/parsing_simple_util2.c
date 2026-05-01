/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:46:50 by akkim             #+#    #+#             */
/*   Updated: 2026/05/01 23:19:34 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "quote.h"

void	add_expanded_args(t_simple_command *cmd, char *token, int *j)
{
	char	**new_args;
	int		i;

	if (!token || token[0] == '\0')
		return ;
	new_args = ft_calloc(*j + 2, sizeof(char *));
	if (!new_args)
		return ;
	i = 0;
	while (i < *j)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[*j] = ft_strdup(token);
	free(cmd->args);
	cmd->args = new_args;
	if (*j == 0)
		cmd->cmd = ft_strdup(token);
	(*j)++;
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
		//remove_quotes(split[j]);
		j++;
	}
	split[j] = NULL;
	return (split);
}
