/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:09 by akkim             #+#    #+#             */
/*   Updated: 2026/02/20 19:46:13 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

// init env & node
void	init_env(t_info_env *env, char **envp);
t_env	*new_env_node(char *key, char *value);

// add & remove env node
void	env_add_back(t_env **head, t_env *new);
void	remove_env(t_info_env *env, char *key);

// find & get env
t_env	*find_env_node(t_env *head, char *key);
char	*get_env_val(t_env *head, char *key);

// free all env
void	all_free_env(t_env *head);

#endif