/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:23:46 by akkim             #+#    #+#             */
/*   Updated: 2026/04/15 20:59:51 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// redirect test function
void	test_redirect(t_redirect *redirect)
{
	if (!redirect)
	return ;
	printf(">> redirect <<");
	printf("type : %s\n", redirect->type);
	printf("filename : %s\n", redirect->filename);
}

// simple_command test function
void	test_simple_command(t_simple_command *simple_command)
{
	int	i;

	i = 0;
	if (!simple_command)
		return ;
	printf(">> simple_command <<\n");
	printf("cmd : %s\n", simple_command->cmd);
	printf(("args :"));
	while (simple_command->args[i])
	{
		printf("%s ", simple_command->args[i]);
		i++;
	}
	printf("\n");
	printf("pre_red :\n");
	test_redirect(simple_command->pre_red);
	printf("suff_red :\n");
	test_redirect(simple_command->suff_red);
}

// pipe_line test function
void	test_pipeline(t_pipeline *pipeline)
{
	if (!pipeline)
		return ;
	printf(">> pipe_line <<\n");
	test_pipeline(pipeline->next);
	test_simple_command(pipeline->simple_command);
}

// command_line test function
void	test_command_line(t_command_line *command_line)
{
	if (!command_line)
		return ;
	printf(">> command_line <<\n");
	printf("comm_oper : %c\n", command_line->comm_oper);
	printf("pipleline : ");
	test_pipeline(command_line->pipeline);
	printf("next : ");
	test_command_line(command_line->next);
}