/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:09 by akkim             #+#    #+#             */
/*   Updated: 2026/02/07 20:13:56 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "ft_printf_bonus.h"
# include <stdio.h>
# include <unistd.h>

typedef struct l_env
{
	char			*key;
	char			*value;
	struct l_env	*next;
}	t_env;

typedef struct l_info_env
{
	t_env	*head;
	int		exit_code;
}		t_info_env;

void	init_env(t_info_env *env, char **envp);
char	*get_env_val(t_env *head, char *key);
void	mini_pwd(t_info_env *env);
void	mini_env(t_info_env *env);
void	mini_export(t_info_env *env, char *str);

#endif