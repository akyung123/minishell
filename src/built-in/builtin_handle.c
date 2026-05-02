/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 17:38:13 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 19:10:51 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	builtin_handler(t_info_env *env, t_simple_command *simple_command)
{
	env->exit_code = 0;
	if (ft_strcmp(simple_command->cmd, "pwd") == 0)
		mini_pwd(env);
	else if (ft_strcmp(simple_command->cmd, "env") == 0)
		mini_env(env);
	else if (ft_strcmp(simple_command->cmd, "echo") == 0)
		mini_echo(env, simple_command->args);
	else if (ft_strcmp(simple_command->cmd, "export") == 0)
	{
		mini_export(env, simple_command->args);
		update_envp_array(env);
	}
	else if (ft_strcmp(simple_command->cmd, "unset") == 0)
	{
		mini_unset(env, simple_command->args);
		update_envp_array(env);
	}
	else if (ft_strcmp(simple_command->cmd, "cd") == 0)
		mini_cd(env, simple_command->args[1]);
	else if (ft_strcmp(simple_command->cmd, "exit") == 0)
		mini_exit(env, simple_command->args);
}
