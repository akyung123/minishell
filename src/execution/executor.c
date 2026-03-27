/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:43:07 by akkim             #+#    #+#             */
/*   Updated: 2026/03/24 19:34:18 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/* 파이프라인 실행에서 리디렉션이 있으면 리디렉션과 피이프로 둘 다 출력됨*/

// simpel_command 실행 함수
void	executor_simple_command(t_simple_command *simple_command)
{
	
}

int	executor_command_line(t_command_line	*command_line)
{
	int	state;

	if (!command_line)
		return (0);
	state = execuotr_pipeline(command_line->pipeline);
	if (command_line->comm_oper == '&')
		if (state) // 성공하면
			execuotr_command_line(command_line->next);
	else if (command_line->comm_oper == '|')
		if (!state) // 실패하면
			execuotr_command_line(command_line->next);
	return (1);
}