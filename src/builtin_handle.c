/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 17:38:13 by akkim             #+#    #+#             */
/*   Updated: 2026/02/20 19:48:02 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 들어오는 string에 맞춰서 실행시키기
// Simple Command가 들어옴
// Simple Command는 **char *cmd, char **args, Redrect *redirects**로 이루어져있음
// 들어온 simple command를 시할할 수 있게 전달함
// cmd를 비교해서 전달하고, args를 따로 전달하도록함

// 추가적으로 이곳에서 args가 필요 이상으로 많이 들어온 경우를 처리하도록 함

#include "passing.h"
#include "minishell.h"

void	builin_handler(t_info_env *env, t_simple_command *simple_command)
{
	if (ft_strcmp(simple_command->cmd, "pwd") == 0)
		mini_pwd(env);
	if (ft_strcmp(simple_command->cmd, "env") == 0)
		mini_env(env);
	if (ft_strcmp(simple_command->cmd, "echo") == 0)
		mini_echo(env, simple_command->args);
	if (ft_strcmp(simple_command->cmd, "export") == 0)
		mini_export(env, simple_command->args[1]);
	if (ft_strcmp(simple_command->cmd, "unset") == 0)
		mini_unset(env, simple_command->args[1]);
	if (ft_strcmp(simple_command->cmd, "cd") == 0)
		mini_cd(env, simple_command->args[1]);
	if (ft_strcmp(simple_command->cmd, "exit") == 0)
		mini_exit(env, simple_command->args);
}

