/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:49:39 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 06:17:20 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

void	update_env_val(t_env *head, char *key, char *value)
{
	t_env	*node;
	t_env	*last;
	t_env	*new_node;

	if (!head || !key)
		return ;
	node = head;
	last = NULL;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (node->value)
				free(node->value);
			if (value)
				node->value = ft_strdup(value);
			else
				node->value = NULL;
			return ;
		}
		if (!node->next)
			last = node;
		node = node->next;
	}
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->key = ft_strdup(value);
	new_node->next = NULL;
	if (last)
		last->next = new_node;
}

void	mini_cd(t_info_env *env, char *path)
{
	char	*prv;
	char	*curr;
	t_env	*tmp;
	t_env	*oldpwd_node;
	char	*target_path;

	if (!path)
	{
		tmp = find_env_node(env->head, "HOME");
		if (!tmp || !tmp->value)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			env->exit_code = 1;
			return ;
		}
		target_path = tmp->value;
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		oldpwd_node = find_env_node(env->head, "OLDPWD");
		if (!oldpwd_node || !oldpwd_node->value)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			env->exit_code = 1;
			return ;
		}
		target_path = oldpwd_node->value;
		ft_putendl_fd(target_path, 1);
	}
	else
		target_path = path;
	if (chdir(target_path) == 0)
	{
		prv = get_env_val(env->head, "PWD");
		update_env_val(env->head, "OLDPWD", prv);
		curr = getcwd(NULL, 0);
		update_env_val(env->head, "PWD", curr);
		free(curr);
		env->exit_code = 0;
		return ;
	}
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	env->exit_code = 1;
	return ;
}
