/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:19 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 21:05:37 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
/*
	parsing 구조 정리
	1. "" 나 ''에 맞춰서 검사한다.
		- 끝나지 않으면 "dquote>" 출력하여 readline으로 받아서 합쳐야함
	2. 받은 전체를 command_line 구조체로 분리한다. && 와 || 를 확인하고 분리해야함
	3. 
*/

	// "", '' 검사 함수
	// "" or ''가 있고 끝나는 지 검사하기
	// 닫히지 않았으면 "dquote>" 출력하여 readline으로 받아서 합치기
	// && 나 ||가 있으면 기준으로 command line이 분리하기
	// 다시 parsing command하기
	// pipeline으로 쪼개야함
	// command_line 분리가 잘 되는지 확인할 것

// 검사 함수
int	flag_q(char c)
{
	if (c == '\'')
		return (1);
	else if (c == '\"')
		return (2);
	return (0);
}

// 현재 따옴표의 끝(닫히는 곳) 인덱스를 찾아서 반환하는 함수
int	skip_quotes(char *line, int i)
{
	int	q;

	q = flag_q(line[i]);
	i++;
	while (line[i] && flag_q(line[i]) != q)
		i++;
	return (i);
}

// 현재 인덱스가 && 또는 || 인지 확인하는 함수
int	is_logical_operator(char *line, int i)
{
	if (line[i] == '&' && line[i + 1] == '&')
		return (1);
	if (line[i] == '|' && line[i + 1] == '|')
		return (1);
	return (0);
}

// 따옴표를 기준으로 해야함. 
// 따옴표 밖일 때만 연산자 검사를 진행하기
// flag 저장해서 구분
// 1 : '\'' 2: '\"'
// 0 : 없음
char	*find_next_operator(char *line)
{
	char	*and_ptr;
	char	*or_ptr;
	int		flag;
	int		i;

	i = 0;
	while (line[i])
	{
		if (flag_q(line[i]))
			i = skip_quotes(line, i);
		else if (is_logical_operator(line, i))
			return (&line[i]);
		if (line[i])
			i++;
	}
	return (NULL);
}

// 따옴표 검사
// 1. 따옴표가 닫혀있는지 검사하고 재입력을 받아서 line을 완성시키기
// 2. 따옴표를 기준으로 분할
t_command_line	*parsing_command_line(char *line)
{
	t_command_line	*command_line;
	char			*comm;
	char			*tmp;

	if (!line)
		return (NULL);
	command_line = malloc(sizeof(t_command_line));
	if (!command_line)
		return (NULL);
	command_line->next = 0;
	command_line->comm_oper = 0;
	comm = find_next_operator(line);
	if (comm != 0)
	{
		command_line->comm_oper = comm[0];
		tmp = ft_substr(line, 0, comm - line);
		command_line->pipeline = parsing_pipeline(tmp);
		free(tmp);
		tmp = ft_strdup(comm + 2);
		command_line->next = parsing_command_line(tmp);
		free(tmp);
	}
	else
		command_line->pipeline = parsing_pipeline(ft_strdup(line));
	return (command_line);
}
