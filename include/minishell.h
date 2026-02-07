/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:09 by akkim             #+#    #+#             */
/*   Updated: 2026/02/07 16:21:12 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "ft_printf_bonus.h"

typedef struct l_env
{
	char	**path;
	char	*user;
	char	*pwd;
	char	*old_pwd;
	int		exit_code;
}	t_env;

void mini_echo(char *str, t_env *env);

#endif