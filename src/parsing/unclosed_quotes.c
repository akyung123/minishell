/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:05:56 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 21:32:25 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// 반환값: 0(다 닫힘), 1(작은따옴표 열림), 2(큰따옴표 열림)
int	check_quote_state(char *line)
{
	int	i;
	int	state;

	i = 0;
	state = 0;
	while (line[i])
	{
		// 닫혀있는 상태에서 따옴표를 만나면 상태 변경
		if (state == 0 && flag_q(line[i]))
			state = flag_q(line[i]);
		// 열려있는 상태에서 같은 짝의 따옴표를 만나면 닫힘(0)
		else if (state != 0 && flag_q(line[i]) == state)
			state = 0;
		i++;
	}
	return (state);
}
char	*handle_unclose_quotes(char *line)
{
	int	state;

	// state : " 혹은 '가 존재하는지 검사함
	// 
	while (state != 0)
	{
		
	}
}
// ft_strjoin 등에 필요한 헤더가 있다고 가정합니다.
char	*handle_unclosed_quotes(char *line)
{
	int		state;
	char	*more;
	char	*tmp1;
	char	*tmp2;

	state = check_quote_state(line);
	while (state != 0)
	{
		if (state == 1)
			more = readline("quote> ");
		else
			more = readline("dquote> ");
			
		// Ctrl+D (EOF) 처리: bash와 똑같은 에러 출력 후 종료
		if (!more)
		{
			printf("minishell: unexpected EOF while looking for matching quote\n");
			free(line);
			return (NULL);
		}
		
		// 기존 line + "\n" + 새로 받은 more 합치기
		tmp1 = ft_strjoin(line, "\n");
		tmp2 = ft_strjoin(tmp1, more);
		free(tmp1);
		free(line);
		free(more);
		
		line = tmp2; // 합쳐진 문자열로 line 갱신
		state = check_quote_state(line); // 상태 다시 검사
	}
	return (line);
}