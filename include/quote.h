/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:35:12 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 01:16:09 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

# include "minishell.h"
# include "parsing.h"

void	check_quote(char **line, t_info_env *env);
int		skip_quote(char **line, int start_i, char type);
void	update_quote_state(char c, int *state);
int		validate_quotes(char *line);
int		check_syntax_error(char *line);

char	*ft_strjoin_free(char *s1, char *s2);
char	*append_char(char *str, char c);
void	process_backticks(char **line, t_info_env *env);
void	restore_expanded_quotes(char *str);

#endif