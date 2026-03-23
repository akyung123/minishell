/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:01:33 by akkim             #+#    #+#             */
/*   Updated: 2026/03/23 02:05:22 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "quote.h"

// >이면 >나 ' '이 와야함
// 두 개가 아닌 모든 것은 공백 추가
// <이면 <나 ' '이 이하 동일
// 따옴표도 확인해야함

// // 비트 마스크로 state update
void	update_quote_state(char c, int *state)
{
	if (c == '\'' && !(*state & 2) && !(*state & 4))
		*state ^= 1;
	else if (c == '\"' && !(*state & 1) && !(*state & 4))
		*state ^= 2;
	else if (c == '`' && !(*state & 1))
		*state ^= 4;
}

int	get_padded_len(char *line)
{
	int	i;
	int	len;
	int	state;

	i = 0;
	len = 0;
	state = 0;
	while (line[i])
	{
		update_quote_state(line[i], &state);
		if (!state && (line[i] == '<' || line[i] == '>'))
		{
			len += 2;
			if (line[i + 1] == line[i])
				len += (++i * 0) + 1;
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
char	*add_space_around_redir(char *line)
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
		if (!chk_quote(line[i]))
			state = 1;
		else
			state = 0;
		if (!state && (line[i] == '<' || line[i] == '>'))
			insert_redir_space(space, line, &i, &j);
		else
			space[j++] = line[i];
	}
	space[j] = '\0';
	return (space);

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
// 따옴ㅍ 제거 함수
// static char	*remove_quotes(char *str)
// {
// 	char	*new;
// 	int		i;
// 	int		j;
// 	int		state;
// 	int 	prev_state;

// 	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
// 	if (!new)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	state = 0;
// 	while (str[i])
// 	{
// 		prev_state = state;
// 		update_quote_state(str[i], &state);
// 		if (state != prev_state)
// 		{
// 			i++;
// 			continue ;
// 		}
// 		new[j++] = str[i++];
// 	}
// 	new[j] = '\0';
// 	return (new);
// }

// 토큰화 함수
// 내부에서 add_space_around_redir 후 ft_split 실행
char	**tokenize_line(char *line)
{
	char	**tokens;
	// char	*tmp;
	char	*padded_line;
	// int		i;

	padded_line = add_space_around_redir(line);
	if (!padded_line)
		return (NULL);
	tokens = split_quotes(padded_line);
	free(padded_line);
	if (!tokens)
		return (NULL);
	// 이 때, 따옴표 제거 X
	// i = 0;
	// while (tokens[i])
	// {
	// 	tmp = tokens[i];
	// 	tokens[i] = remove_quotes(tmp);
	// 	free(tmp);
	// 	i++;
	// }
	return (tokens);
}
