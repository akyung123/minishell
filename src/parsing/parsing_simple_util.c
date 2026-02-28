/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:01:33 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 20:19:21 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// >이면 >나 ' '이 와야함
// 두 개가 아닌 모든 것은 공백 추가
// <이면 <나 ' '이 이하 동일
// 따옴표도 확인해야함

// 공백을 추가한 문자열의 길이 함수
int	get_padded_len(char *line)
{
	// 리다이렌션 기호이면 다음 문자가 ' '인지 검사하고
	// 공백이 아니면 ' '를 추가하여 길이를 잼
	int	len;
	int	i;

	i = 0;
	len = 0;
	while (line[i])
	{
		// 따옴표 처리 해야함.... 근
		if (line[i])
		{
			
		}
	}
	return (len);
}
int	get_padded_len(char *line)
{
	int	i = 0;
	int	len = 0;
	int	sq = 0; // 작은따옴표 상태
	int	dq = 0; // 큰따옴표 상태

	while (line[i])
	{
		update_quote_state(line[i], &sq, &dq);
		if (!sq && !dq && (line[i] == '<' || line[i] == '>'))
		{
			len += 2; // 양옆 공백 2칸 추가
			if (line[i + 1] == line[i]) // ">>" 나 "<<" 처럼 두 개짜리면
			{
				len++; // 두 번째 기호 길이 1칸 추가
				i++;   // 한 칸 미리 건너뜀
			}
			len++; // 첫 번째 기호 길이 1칸 추가
		}
		else
			len++; // 일반 문자
		i++;
	}
	return (len);
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
