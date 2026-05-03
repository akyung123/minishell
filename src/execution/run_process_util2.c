/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_process_util2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:14:46 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:10:53 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"

void	run_execve(t_pipex *pipex, t_simple_command *simple)
{
	if (execve(simple->cmd, simple->args, pipex->envp) == -1)
	{
		ft_printf("cmd : %s\n args[0] : %s\n", simple->cmd, simple->args[0]);
		perror(simple->cmd);
		if (pipex->cmd_line)
			free_command_line(pipex->cmd_line);
		free_pipex(pipex);
		exit(127);
	}
}
