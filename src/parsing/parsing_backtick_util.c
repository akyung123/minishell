/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_backtick_util.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 00:58:26 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 07:44:55 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
#include "execution.h"
#include "quote.h"

char	*read_from_pipe(int fd)
{
	char	buffer[1024];
	int		bytes_read;
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (1)
	{
		bytes_read = read(fd, buffer, 1023);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(result, buffer);
		free(result);
		result = tmp;
	}
	close(fd);
	return (result);
}

void	replace_newline_to_space(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == '\n')
			str[i] = ' ';
		i++;
	}
}

void	hide_expanded_quotes(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == '\'')
			str[i] = 1;
		else if (str[i] == '\"')
			str[i] = 2;
		i++;
	}
}

void	restore_expanded_quotes(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == 1)
			str[i] = '\'';
		else if (str[i] == 2)
			str[i] = '\"';
		i++;
	}
}

char	*execute_backtick(char *inner_cmd, t_info_env *env)
{
	pid_t		pid;
	char		*result;
	t_pipex		*pipex;
	t_pipeline	*sub;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (pipe(pipex->fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
	{
		close(pipex->fd[0]);
		dup2(pipex->fd[1], STDOUT_FILENO);
		close(pipex->fd[1]);
		sub = parsing_pipeline(env, inner_cmd);
		if (sub)
			executor_pipeline(env, sub, pipex);
		exit(0);
	}
	close(pipex->fd[1]);
	result = read_from_pipe(pipex->fd[0]);
	waitpid(pid, NULL, 0);
	replace_newline_to_space(result);
	hide_expanded_quotes(result);
	return (result);
}
