/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:01:33 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 13:28:42 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// 공백을 추가한 문자열의 길이 함수
int	get_padded_len(char *line)
{
	// 리다이렌션 기호이면 다음 문자가 ' '인지 검사하고
	// 공백이 아니면 ' '를 추가하여 길이를 잼
	
}
// 공백 추가 함수
char	*add_space_around_redir(char *line)
{
	char	*res;
	int		len;

	len = get_padded_len(line);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	return (res);
}

// 공백기준으로 쪼개는 함수
// but, ft_split과 다르게 "" 안의 공백은 무시해야함
char	**split_quotes()
{
	
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
	tokens = split_quotes();
	free(padded_line);
	return (tokens);
}
