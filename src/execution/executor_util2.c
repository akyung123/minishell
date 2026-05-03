/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_util2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:02:56 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 14:14:27 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "execution.h"
#include "minishell.h"
#include "quote.h"

static void	read_here_doc(int fd, char *eof, t_info_env *env, t_redirect *rd)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited "
				"by end-of-file (wanted `", 2);
			ft_putstr_fd(rd->filename, 2);
			ft_putstr_fd("')\n", 2);
			return ;
		}
		if (rd->flag == 1)
			expand_env(&line, env);
		if (!ft_strncmp(line, eof, ft_strlen(eof)))
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
}

int	here_doc(t_info_env *env, t_redirect *redirect)
{
	int		fd;
	char	*eof;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	eof = ft_strjoin(redirect->filename, "\n");
	read_here_doc(fd, eof, env, redirect);
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	unlink(".here_doc");
	free(eof);
	return (fd);
}

// redirect에 맞게 파일 열기
int	open_file_red(t_info_env *env, t_redirect *redirect)
{
	int	fd;

	if (ft_strcmp(redirect->type, "<<") == 0)
		fd = here_doc(env, redirect);
	else if (ft_strcmp(redirect->type, ">>") == 0)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strcmp(redirect->type, "<") == 0)
		fd = open(redirect->filename, O_RDONLY);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

void	setting_command(t_info_env *env, t_pipex *pipex,
			t_simple_command *simple_command)
{
	t_redirect	*tmp;

	tmp = simple_command->red;
	while (tmp)
	{
		if (tmp && tmp->type && (!ft_strcmp(tmp->type, "<<")
				|| !ft_strcmp(tmp->type, "<")))
		{
			pipex->in = open_file_red(env, tmp);
			if (pipex->in == -1)
				perror(tmp->filename);
		}
		else
		{
			pipex->out = open_file_red(env, tmp);
			if (pipex->out == -1)
			{
				perror(tmp->filename);
				exit(1);
			}
		}
		tmp = tmp->next;
	}
}

int	exec_single_builtin(t_info_env *env, t_pipeline *pl, t_pipex *px)
{
	int	stdout_backup;
	int	ac;

	stdout_backup = dup(STDOUT_FILENO);
	setting_command(env, px, pl->simple_command);
	set_process(px);
	builtin_handler(env, pl->simple_command);
	ac = 0;
	while (pl->simple_command->args && pl->simple_command->args[ac])
		ac++;
	if (ac > 0)
		update_last_arg(env, pl->simple_command->args[ac - 1]);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	px->pipeline_status = (env->exit_code == 0);
	return (px->pipeline_status);
}
