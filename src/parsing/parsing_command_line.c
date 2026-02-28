/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:19 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 13:22:07 by akkim            ###   ########.fr       */
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
	
	// t_redirect	*extract_redirections(char *line, char **clean_line)
	// {
	// 	t_redirect	*head;
	// 	char		*tmp;
	// 	int			i;
	
	// 	head = 0;
	// 	tmp = ft_strdup(line);
	// 	i = 0;
	// 	while (tmp && tmp[i])
	// 	{
	// 		if (tmp[i] == '>' || tmp[i] == '<')
	// 		{
	// 			add_redir_node(&head, &tmp[i]);
	// 			mask_redirection_part(tmp, i);
	// 		}
	// 		i++;
	// 	}
	// 	*clean_line = tmp;
	// 	return (head);
	// }

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
