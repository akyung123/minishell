/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 18:32:27 by akkim             #+#    #+#             */
/*   Updated: 2026/02/27 23:59:12 by akkim            ###   ########.fr       */
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

char	*find_next_operator(char *line)
{
	char	*and_ptr;
	char	*or_ptr;

	and_ptr = ft_strstr(line, "&&");
	or_ptr = ft_strstr(line, "||");
	if (and_ptr != 0 && or_ptr != 0)
	{
		if (and_ptr < or_ptr)
			return (and_ptr);
		return (or_ptr);
	}
	if (and_ptr != 0)
		return (and_ptr);
	return (or_ptr);
}
t_redirect	*extract_redirections(char *line, char **clean_line)
{
	t_redirect	*head;
	char		*tmp;
	int			i;

	head = 0;
	tmp = ft_strdup(line);
	i = 0;
	while (tmp && tmp[i])
	{
		if (tmp[i] == '>' || tmp[i] == '<')
		{
			add_redir_node(&head, &tmp[i]);
			mask_redirection_part(tmp, i);
		}
		i++;
	}
	*clean_line = tmp;
	return (head);
}
// redirect 찾기
int	is_redir(char *token)
{
	// 토큰이 4가지 기호 중 하나와 완벽히 일치하면 1(true) 반환
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 ||
		ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
	{
		return (1);
	}
	return (0);
}

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

	// padding할 문자열 길이 재기
	retrun (res)
}

// 공백기준으로 쪼개는 함수
// but, ft_split과 다르게 "" 안의 공백은 무시해야함
char	**split_quotes()
{
	
}

// 토큰화 함수
char	**tokenize_line(char *line)
{
	char	*padded_line;
	char	**tokens;

	// 공백 띄우기 함수
	padded_line = add_space_around_redir(line);
	if (!padded_line)
		return (NULL);
	tokens = split_quotes();
	free(padded_line);
	return (tokens);
}
// 문법 체크 함수
int check_syntax(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens[0])
		retrun (0);
	while (tokens[i])
	{
		if (is_redir(tokens[i]))
		{
			if (tokens[i + 1] == NULL) // 파일명 x
				return (0);
			else if (is_redir(tokens[i + 1])) // 기호 뒤에 기호
				return (0);
			i += 2;
		}
		else
			i++;
	}
	return (1);
}

// 구조체에 데이터 채우기
t_simple_command	*build_cmd_struct(char **tokens)
{
	
}

// ' '를 추가 후, 토큰화 -> 문법 검사 필요!
// <simple_command> ::= <cmd_name> <arguments_opt> <redirects_opt>
t_simple_command	*parsing_simple_command(char *line)
{
	t_simple_command	*simple_command;
	char				*tmp;
	char				**tokens;

	if (!line)
		return (NULL);
	simple_command = malloc(siezeof(simple_command));
	if (!simple_command)
		return (NULL);
	// 내부에서 add_space_around_redir 후 ft_split 실행
	tokens = tokenize_line(line);
	// 문법 검사
	if (!check_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	// 구조체에 데이터 채우가
	simple_command = build_cmd_struct(tokens);
	return (simple_command);
}

// p_op : pipe operator
t_pipeline	*parsing_pipeline(char *line)
{
	t_pipeline	*pipeline;
	char		*tmp;
	char		*p_op;

	if (!line)
		return (NULL);
	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	pipeline->next = 0;
	p_op = ft_strrchr(line, '|');
	if (p_op != NULL)
	{
		tmp = ft_substr(line, 0, p_op - line);
		pipeline->next = parsing_pipeline(tmp);
		free(tmp);
		tmp = ft_strdup(p_op + 1);
		pipeline->simple_command = parsing_simple_command(tmp);
		free(tmp);
	}
	else
		pipeline->simple_command = parsing_simple_command(line);
	return (pipeline);
}

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
