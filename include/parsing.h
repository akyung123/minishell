/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 18:00:44 by akkim             #+#    #+#             */
/*   Updated: 2026/03/23 16:50:42 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

typedef struct l_redirect
{
	char	*type;
	char	*filename;
	struct l_redirect   *next;
}	t_redirect;

typedef struct l_simple_command
{
	char		*cmd;
	char		**args;
	t_redirect	*pre_red;
	t_redirect	*suff_red;

}	t_simple_command;

typedef struct l_pipeline
{
	t_simple_command	*simple_command;
	struct l_pipeline	*next;
}	t_pipeline;

typedef struct l_command_line
{
	char					comm_oper;	// '&&' or '||' or 'None'
	t_pipeline				*pipeline;	// 왼쪽 파이프 라인 **항상 존재함**
	struct l_command_line	*next;	// 오른쪽 코멘트 라인 ** 존재 or 'None'** None되면 line 종료
}	t_command_line;

char	*minishell_strrchr(const char *str, int ch);
char	*ft_strstr(const char *haystack, const char *needle);

void	check_quote(char **line);

t_command_line		*parsing_command_line(char **line);
t_pipeline			*parsing_pipeline(char *line);
t_simple_command	*parsing_simple_command(char *line);
char				**tokenize_line(char *line);

void	builin_handler(t_info_env *env, t_simple_command *simple_command);
int		is_redir(char *token);

void test_command_line(t_command_line *command_line);

#endif