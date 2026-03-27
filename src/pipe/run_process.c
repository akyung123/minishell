/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:55:39 by akkim             #+#    #+#             */
/*   Updated: 2026/03/24 19:56:50 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	set_last_child(t_pipex *pipex)
{
	dup2(pipex->in, 0);
	close(pipex->in);
	dup2(pipex->out, 1);
	close(pipex->out);
	close(pipex->fd[1]);
	close(pipex->fd[0]);
}

void	set_child(t_pipex *pipex)
{
	int	temp;

	if (pipex->in == -1)
	{
		temp = open("/dev/null", O_RDONLY);
		dup2(temp, 0);
		close(temp);
	}
	else
	{
		dup2(pipex->in, 0);
		close(pipex->in);
	}
	dup2(pipex->fd[1], 1);
	close(pipex->fd[1]);
	close(pipex->fd[0]);
	close(pipex->out);
}

void	child_process(t_pipex *pipex, char *cmd)
{
	set_child(pipex);
	if (pipex->in == -1)
	{
		free_split(pipex->paths);
		free(pipex);
		exit(1);
	}
	run_cmd(pipex, cmd);
}

void	last_process(t_pipex *pipex, char *cmd)
{
	set_last_child(pipex);
	run_cmd(pipex, cmd);
}

// make pipe && fork
pid_t	run_process(t_pipex *pipex, int argc, char *cmd)
{
	pid_t	pid;

	if (pipex->i < argc - 2)
		pipe(pipex->fd);
	pid = fork();
	if (pid == 0)
	{
		// set process
		if (pipex->i < argc - 2)
			child_process(pipex, cmd);
		else
			last_process(pipex, cmd);
	}
	if (pipex->i == argc - 2)
		close(pipex->in);
	if (pipex->i < argc - 2)
	{
		close(pipex->fd[1]);
		if (pipex->in != -1)
			close(pipex->in);
		pipex->in = pipex->fd[0];
	}
	return (pid);
}
