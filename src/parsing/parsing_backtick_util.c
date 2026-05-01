/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_backtick_util.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 00:58:26 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 01:50:15 by akkim            ###   ########.fr       */
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
	int			fd[2];
	t_pipex		sub_pipex;
	t_pipeline	*sub;
	int			status;

	if (pipe(fd) == -1)
		return (ft_strdup(""));
	pid = fork();
	if (pid == 0)
	{
		ft_printf("execute-inner : %s\n", inner_cmd);
		sub = parsing_pipeline(env, inner_cmd);
		ft_printf("cmd : %s\n", sub->simple_command->cmd);
		set_pipex(env, &sub_pipex);
		if (!sub)
			exit(1);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		ft_memset(&sub_pipex, 0, sizeof(t_pipex));
		// sub_pipex.stdout_backup = dup(STDOUT_FILENO);
		// sub_pipex.stdin_backup = dup(STDIN_FILENO);
		executor_pipeline(env, sub, &sub_pipex);
		// close(sub_pipex.stdout_backup);
		// close(sub_pipex.stdin_backup);
		while (wait(&status) > 0)
			;
		exit(env->exit_code);
	}
	close(fd[1]);
	result = read_from_pipe(fd[0]);
	waitpid(pid, NULL, 0);
	if (!result)
		result = ft_strdup("");
	printf("DEBUG: backtick result = [%s]\n", result);
	replace_newline_to_space(result);
	hide_expanded_quotes(result);
	return (result);
}
