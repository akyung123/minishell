/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:35:12 by akkim             #+#    #+#             */
/*   Updated: 2026/03/24 15:56:23 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_H
# define QUOTE_H

# include "minishell.h"
# include "parsing.h"

int	chk_quote(char q);
int	skip_quote(char **line, int start_i, char type);
void	update_quote_state(char c, int *state);
int	validate_quotes(char *line);

#endif