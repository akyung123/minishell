/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:35:12 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 14:39:14 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

# include "minishell.h"
# include "parsing.h"

typedef struct s_parse_info
{
	char	**res;
	char	*line;
	int		i;
	int		*state;
}	t_parse_info;

// chek quote
void	check_quote(char **line, t_info_env *env);
void	update_quote_state(char c, int *state);

int		check_syntax_error(char *line);
// print error 
int		syntax_err_msg(char *token);

// parsing simple command
	// tokenize_line function
char	**split_quotes(char *s);
	// build cmd
int		is_redir(char *token);
void	refine_line(char **line, t_info_env *env);
void	add_expanded_args(t_simple_command *cmd, char *token, int *j);

// parsing backtick
char	*execute_backtick(char *inner_cmd, t_info_env *env);

void	free_tokens(char **tokens);
int		skip_quote(char **line, int start_i, char type);
int		validate_quotes(char *line);

char	*ft_strjoin_free(char *s1, char *s2);
char	*append_char(char *str, char c);
void	process_backticks(char **line, t_info_env *env);
void	restore_expanded_quotes(char *str);

void	handle_redirection(t_simple_command	*cmd, char **tokens,
			t_info_env *env, int *i);

int		handle_ansi_c_quoting(char **res, char *line, int i);
int		handle_dollar(char **res, char *str, t_info_env *env, int st);
int		parse_hex_escape(char *line, int *i, int max_len);

#endif