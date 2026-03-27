/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:12:21 by akkim             #+#    #+#             */
/*   Updated: 2026/03/27 16:49:25 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "minishell.h"
#include "parsing.h"
#include "env.h"

// 각 redirect에 맞춰서 파일 열기
void	open_redirect(t_pipex *pipex, t_redirect *redirect)
{
	
}

int	here_doc(char **argv, int prev_fd)
{
	char	*line;
	char	*eof;

	prev_fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	eof = ft_strjoin(argv[2], "\n");
	write(1, "> ", 2);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		if (!ft_strncmp(line, eof, ft_strlen(eof)))
		{
			free(line);
			break ;
		}
		write(prev_fd, line, ft_strlen(line));
		free(line);
		write(1, "> ", 2);
	}
	free(eof);
	close(prev_fd);
	prev_fd = open(".here_doc", O_RDONLY);
	unlink(".here_doc");
	return (prev_fd);
}

void	setting_here_doc(t_pipex *pipex, t_simple_command *simple_command)
{
	pipex->i++;
	pipex->in = here_doc(argv, pipex->in);
	pipex->out = open(simple_command->suff_red, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (pipex->out == -1)
	{
		perror(argv[argc -1]);
		close(pipex->in);
		close(pipex->out);
		free_split(pipex->paths);
		free(pipex);
		exit(1);
	}
}

// redirect에 맞는 함수 생성
void	setting_defalt(t_pipex *pipex, t_simple_command	*simple_command)
{
	pipex->in = open(simple_command->pre_red, O_RDONLY);
	if (pipex->in == -1)
		perror(simple_command->pre_red);
	pipex->out = open(simple_command->suff_red,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out == -1)
	{
		perror(simple_command->pre_red);
		close(pipex->in);
		close(pipex->out);
		free_split(pipex->paths);
		free(pipex);
		exit(1);
	}
}

void	setting_piex(t_info_env *env, t_pipex *pipex, t_simple_command	*simple_command)
{
	pipex->i = 2;
	pipex->in = -1;
	pipex->out = -1;
	pipex->paths = find_env_node(env->head, "PATH");
	if (!strcmp(argv[1], "here_doc"))
		setting_here_doc(pipex, simple_command);
	else
		setting_defalt(pipex, simple_command);
}

