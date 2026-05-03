/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 18:00:44 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:03:03 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

typedef struct l_redirect
{
	char				*type;
	char				*filename;
	int					flag;
	struct l_redirect	*next;
}	t_redirect;

typedef struct l_simple_command
{
	char		*line;
	char		*cmd;
	char		**args;
	t_redirect	*red;
}	t_simple_command;

typedef struct l_pipeline
{
	t_simple_command	*simple_command;
	struct l_pipeline	*next;
}	t_pipeline;

typedef struct l_command_line
{
	char					comm_oper;
	t_pipeline				*pipeline;
	struct l_command_line	*next;
}	t_command_line;

// util
char				*minishell_strrchr(const char *str, int ch);
char				*ft_strstr(const char *haystack, const char *needle);

t_command_line		*parsing_command_line(t_info_env *env, char **line);
t_pipeline			*parsing_pipeline(t_info_env *env, char *line);
t_simple_command	*parsing_simple_command(t_info_env *env, char *line);
char				**tokenize_line(char *line);
t_simple_command	*build_cmd_struct(char **tokens, t_info_env *env);

// free
char				**free_split(char **str);
char				*ft_strjoin_free(char *s1, char *s2);

void				remove_quotes(char *line);
void				builtin_handler(t_info_env *env,
						t_simple_command *simple_command);
void				expand_env(char **line, t_info_env *env);

void				handle_redirection(t_simple_command	*cmd, char **tokens,
						t_info_env *env, int *i);

#endif