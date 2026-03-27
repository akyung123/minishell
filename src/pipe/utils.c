/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 02:38:47 by akkim             #+#    #+#             */
/*   Updated: 2025/12/30 21:00:29 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
void	cmd_error(t_pipex *pipex, char **cmd, int chk)
{
	if (chk == 2)
	{
		free_split(pipex->paths);
		free(pipex);
		exit(0);
	}
	if (!chk)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putchar_fd('\n', 2);
	}
	else if (chk == 3)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": no such file or directory\n", 2);
		ft_putchar_fd('\n', 2);
	}
	else
		ft_putstr_fd("pipex: command not found: ", 2);
	free_split(cmd);
	free_split(pipex->paths);
	free(pipex);
	exit(127);
}

void	run_execve(t_pipex *pipex, char **cmd_full)
{
	if (execve(cmd_full[0], cmd_full, pipex->envp) == -1)
	{
		perror(cmd_full[0]);
		free_split(cmd_full);
		if (errno == EACCES || errno == EISDIR)
			exit(126);
		else if (errno == ENOENT)
			exit(127);
		else
			exit(1);
	}
}

// run cmd
void	run_cmd(t_pipex *pipex, char *cmd)
{
	char	**cmd_full;
	char	*path;

	cmd_full = NULL;
	path = NULL;
	if (!cmd || !*cmd)
		cmd_error(pipex, cmd_full, 2);
	cmd_full = ft_split(cmd, ' ');
	if (!cmd_full)
		cmd_error(pipex, cmd_full, 1);
	if (cmd_full[0][0] == '/'
		|| (cmd_full[0][0] == '.' && cmd_full[0][1] == '/'))
	{
		if (access(cmd_full[0], X_OK) == 0)
			path = cmd_full[0];
		else
			cmd_error(pipex, cmd_full, 3);
	}
	else
		path = find_path(pipex, cmd_full[0]);
	if (!path)
		cmd_error(pipex, cmd_full, 0);
	cmd_full[0] = path;
	run_execve(pipex, cmd_full);
}
