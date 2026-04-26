/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_process_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 07:49:00 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 08:00:34 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "execution.h"

// find path
static char	*find_path(t_pipex *pipex, char *cmd)
{
	int		i;
	char	*str;
	char	*path;

	i = 0;
	while (pipex->paths[i])
	{
		path = ft_strjoin(pipex->paths[i], "/");
		str = ft_strjoin(path, cmd);
		free(path);
		if (!access(str, 00))
			return (str);
		i++;
		free(str);
	}
	return (NULL);
}

// 0 : cmd x, 1: fake cmd
void	cmd_error(t_pipex *pipex, char **cmd)
{
	if (cmd && cmd[0])
		ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_split(cmd);
	free_split(pipex->paths);
	free(pipex);
	exit(127);
}

void	path_error(char **cmd)
{
	if (cmd && cmd[0])
	{
		ft_putstr_fd(cmd[0], 2);
	}
	if (access(cmd[0], F_OK) == 0)
	{
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	ft_putstr_fd(": No such file or directory\n", 2);
	exit(127);
}

void	run_execve(t_pipex *pipex, t_simple_command *simple)
{
	if (execve(simple->cmd, simple->args, pipex->envp) == -1)
	{
		perror(simple->cmd);
		exit(1);
	}
}

void	run_cmd(t_pipex *pipex, t_simple_command *simple)
{
	if (!simple->cmd || !*simple->cmd || !simple->args)
		cmd_error(pipex, simple->args);
	if (ft_strchr(simple->args[0], '/'))
	{
		if (access(simple->cmd, X_OK) != 0)
			path_error(simple->args);
	}
	else
	{
		simple->cmd = find_path(pipex, simple->args[0]);
		if (!simple->cmd)
			cmd_error(pipex, simple->args);
	}
	simple->args[0] = simple->cmd;
	run_execve(pipex, simple);
}
