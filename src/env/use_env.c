/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:46:19 by akkim             #+#    #+#             */
/*   Updated: 2026/05/01 22:53:46 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

t_env	*find_env_all(t_info_env *env, char *key)
{
	t_env	*node;

	node = find_env_node(env->head, key);
	if (node)
		return (node);
	return (find_env_node(env->hide_head, key));
}

char	*get_env_val_all(t_info_env *env, char *key)
{
	t_env	*node;

	node = find_env_all(env, key);
	if (node)
		return (node->value);
	node = find_env_node(env->hide_head, key);
	if (node)
		return (node->value);
	return (NULL);
}

// find node by key
t_env	*find_env_node(t_env *head, char *key)
{
	while (head)
	{
		if (ft_strlen(head->key) == ft_strlen(key)
			&& ft_strncmp(head->key, key, ft_strlen(key)) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

// get value
char	*get_env_val(t_env *head, char *key)
{
	t_env	*node;

	node = find_env_node(head, key);
	if (node)
		return (node->value);
	return (NULL);
}

// add env

// remove env
void	remove_env(t_info_env *env, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = env->head;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				env->head = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
