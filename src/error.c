/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:18:53 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 14:18:37 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "quote.h"
#include "execution.h"

int	print_unexpected_token(char *line, int idx)
{
	char	token[3];

	if (line[idx] == '\0')
		return (syntax_err_msg("newline"));
	ft_memset(token, 0, 3);
	token[0] = line[idx];
	if (line[idx + 1] && line[idx] == line[idx + 1])
		token[1] = line[idx + 1];
	return (syntax_err_msg(token));
}

// 0 : cmd x, 1: fake cmd
void	cmd_error(t_pipex *pipex, char **cmd)
{
	char	*error_msg;

	if (cmd && cmd[0])
	{
		error_msg = ft_strjoin(cmd[0], ": command not found\n");
		if (error_msg)
		{
			ft_putstr_fd(error_msg, 2);
			free(error_msg);
		}
	}
	free_pipex(pipex);
	if (pipex->cmd_line)
		free_command_line(pipex->cmd_line);
	exit(127);
}

void	export_error(t_info_env *env, char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	env->exit_code = 1;
}

int	syntax_err_msg(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}
