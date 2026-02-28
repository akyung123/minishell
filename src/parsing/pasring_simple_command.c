/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasring_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:11 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 14:03:18 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_tokens(char **tokens)
{
	
}

// redirect 찾기
int	is_redir(char *token)
{
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 ||
		ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}


// 문법 체크 함수
// 파일명 x && 기호 뒤에 기호 오는 경우
int	check_syntax(char **tokens)
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
	t_simple_command	*simple_command;

	return (simple_command);
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
	tokens = tokenize_line(line);
	if (!check_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	simple_command = build_cmd_struct(tokens);
	return (simple_command);
}
