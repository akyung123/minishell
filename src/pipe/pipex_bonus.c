/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:11:04 by akkim             #+#    #+#             */
/*   Updated: 2026/03/27 15:58:37 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "parsing.h"

int	handle_exit(t_exit exitcode)
{
	waitpid(exitcode.last_pid, &exitcode.status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(exitcode.status))
	{
		exitcode.last_exit_code = WEXITSTATUS(exitcode.status);
	}
	return (exitcode.last_exit_code);
}

int	pipeline(t_info_env *env, t_pipeline *pipeline)
{
	t_exit	exitcode;
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex) + 1);
	setting_pipex(pipex, pipeline);
	while (pipex->i < argc -1)
	{
		pipex->pid = run_process(pipex, argc, argv[pipex->i]);
		if (pipex->i == argc - 2)
			exitcode.last_pid = pipex->pid;
		pipex->i++;
	}
	close(pipex->in);
	close(pipex->out);
	free_split(pipex->paths);
	free(pipex);
	exit(handle_exit(exitcode));
}
