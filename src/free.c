/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 11:04:14 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 02:11:37 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	free_redirection(t_redirect *redir)
{
	t_redirect	*tmp;
	t_redirect	*tmp2;

	if (!redir)
		return ;
	tmp = redir;
	while (tmp)
	{
		if (tmp->filename)
			free(tmp->filename);
		if (tmp->type)
			free(tmp->type);
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
	}
}

// 2. 단순 명령어(args 배열 등) 해제
void	free_simple_command(t_simple_command *simple)
{
	int	i;

	if (!simple)
		return ;
	if (simple->args)
	{
		i = 0;
		while (simple->args[i])
		{
			free(simple->args[i]);
			i++;
		}
		free(simple->args);
	}
	if (simple->cmd)
		free(simple->cmd);
	if (simple->line)
		free(simple->line);
	free_redirection(simple->red);
	free(simple);
}

// 3. 파이프라인 재귀 해제 (|)
void	free_pipeline(t_pipeline *pipeline)
{
	if (!pipeline)
		return ;
	if (pipeline->next)
		free_pipeline(pipeline->next);
	free_simple_command(pipeline->simple_command);
	free(pipeline);
}

// 4. 최종 커맨드 라인 재귀 해제 (&&, ||)
void	free_command_line(t_command_line *cmd_line)
{
	if (!cmd_line)
		return ;
	if (cmd_line->next)
		free_command_line(cmd_line->next);
	free_pipeline(cmd_line->pipeline);
	free(cmd_line);
}
