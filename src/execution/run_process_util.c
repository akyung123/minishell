/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_process_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 07:49:00 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 19:03:07 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"

// find path
static char	*find_path(t_pipex *pipex, char *cmd)
{
	int		i;
	char	*str;
	char	*path;

	i = 0;
	if (!pipex->paths)
		return (NULL);
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

void	path_error(t_pipex *pipex, char **cmd)
{
	if (cmd && cmd[0])
	{
		ft_putstr_fd(cmd[0], 2);
	}
	if (access(cmd[0], F_OK) == 0)
	{
		ft_putstr_fd(": Permission denied\n", 2);
		if (pipex->cmd_line)
			free_command_line(pipex->cmd_line);
		free_pipex(pipex);
		exit(126);
	}
	ft_putstr_fd(": No such file or directory\n", 2);
	if (pipex->cmd_line)
		free_command_line(pipex->cmd_line);
	free_pipex(pipex);
	exit(127);
}

void	check_is_directory(t_pipex *pipex, char *cmd)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			if (pipex->cmd_line)
				free_command_line(pipex->cmd_line);
			free_pipex(pipex);
			exit(1);
		}
	}
}

static void	resolve_cmd_path(t_pipex *pipex, t_simple_command *simple)
{
	char	*tmp_path;

	if (ft_strchr(simple->args[0], '/'))
	{
		if (access(simple->cmd, X_OK) != 0)
			path_error(pipex, simple->args);
	}
	else
	{
		tmp_path = find_path(pipex, simple->cmd);
		if (!tmp_path)
			cmd_error(pipex, simple->args);
		free(simple->cmd);
		simple->cmd = tmp_path;
	}
}

void	run_cmd(t_info_env *env, t_pipex *pipex,
	t_simple_command *simple)
{

	if (is_builtin(simple->cmd))
	{
		builtin_handler(env, simple);
		shell_cleanup(env);
		exit(0);
	}
	if (!simple->cmd || !*simple->cmd || !simple->args)
		cmd_error(pipex, simple->args);
	check_is_directory(pipex, simple->cmd);
	resolve_cmd_path(pipex, simple);
	update_last_arg(env, simple->cmd);
	pipex->envp = env->envp;
	run_execve(pipex, simple);
}
