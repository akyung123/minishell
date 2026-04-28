/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_proccess.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 18:31:59 by akkim             #+#    #+#             */
/*   Updated: 2026/04/28 23:10:29 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "execution.h"

char	**free_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		i++;
	}
	free (str);
	return (NULL);
}

void	set_process(t_pipex *pipex)
{
	if (pipex->in != -1)
	{
		dup2(pipex->in, 0);
		if (pipex->in > 2)
			close(pipex->in);
	}
	if (pipex->out != -1)
	{
		dup2(pipex->out, 1);
		if (pipex->out > 2)
			close(pipex->out);
	}
	else if (pipex->fd[1] != -1)
	{
		dup2(pipex->fd[1], 1);
		if (pipex->in > 2)
			close(pipex->fd[1]);
	}
	if (pipex->fd[0] > 2)
		close(pipex->fd[0]);
}

pid_t	run_process(t_info_env *env, t_pipex *pipex, t_simple_command *simple_command)
{
	pid_t	pid;

	pid = fork();
	(void)env;
	if (pid == 0)
	{
		set_process(pipex);
		run_cmd(pipex, simple_command);
	}
	else
	{
		if (pipex->in > 2)
			close(pipex->in);
		if (pipex->fd[1] > 2)
			close(pipex->fd[1]);
		pipex->in = pipex->fd[0];
	}
	return (pid);
}
