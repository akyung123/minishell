/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_proccess.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+  ㅋ     +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 18:31:59 by akkim             #+#    #+#             */
/*   Updated: 2026/04/15 18:33:09 by akkim            ###   ########.fr       */
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
	ft_putstr_fd("pipex: command not found: ", 2);
	free_split(cmd);
	free_split(pipex->paths);
	free(pipex);
	exit(127);
}

void	run_execve(t_pipex *pipex, t_simple_command *simple)
{
	if (execve(simple->cmd, simple->args, pipex->envp) == -1)
	{
		perror(simple->cmd);
		// erorr 처리
		exit(1);
	}
}

void	run_cmd(t_pipex *pipex, t_simple_command *simple)
{
	if (!simple->cmd || !*simple->cmd)
		cmd_error(pipex, simple->args); // erorr 처리
	if (!simple->args)
		cmd_error(pipex, simple->args);  // erorr 처리
	if (simple->args[0][0] == '/'
		|| (simple->args[0][0] == '.' && simple->args[0][1] == '/'))
	{
		if (access(simple->cmd, X_OK) == 0)
			;
		else
			cmd_error(pipex, simple->args);  // erorr 처리
	}
	else
		simple->cmd = find_path(pipex, simple->args[0]);
	if (!simple->cmd)
		cmd_error(pipex, simple->args);  // erorr 처리
	simple->args[0] = simple->cmd;
	run_execve(pipex, simple);
}

void	set_process(t_pipex *pipex)
{
	//int	temp;

	// if (pipex->in == -1) // 빈 입력이랑 연결
	// {
	// 	temp = open("/dev/null", O_RDONLY);
	// 	dup2(temp, 0);
	// 	close(temp);
	// }
	if (pipex->in != -1)
	{
		dup2(pipex->in, 0);
		close(pipex->in);
	}
	if (pipex->out != -1)
	{
		dup2(pipex->fd[1], 1);
		if (pipex->in > 2)
			close(pipex->fd[1]);
	}
	else
	{
		dup2(pipex->out, 1);
		if (pipex->in > 2)
			close(pipex->out);
	}
	close(pipex->fd[0]);
}

pid_t	run_process(t_pipex *pipex, t_simple_command *simple_command)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		set_process(pipex);
		run_cmd(pipex, simple_command);
	}
	else
	{
		// if (pipex->in != -1)
		// 	close(pipex->in);
		if (pipex->fd[1] != -1)
			close(pipex->fd[1]);
		pipex->in = pipex->fd[0];
	}
	return (pid);
}
