/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env_update_val.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:18:25 by sejang            #+#    #+#             */
/*   Updated: 2026/05/02 23:18:28 by sejang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "env.h"

static void	update_existing_node(t_env *node, char *value)
{
	if (node->value)
		free(node->value);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
}

static void	append_new_node(t_env *last, char *key, char *value)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->key = ft_strdup(value);
	new_node->next = NULL;
	last->next = new_node;
}

void	update_env_val(t_env *head, char *key, char *value)
{
	t_env	*node;
	t_env	*last;

	if (!head || !key)
		return ;
	node = head;
	last = NULL;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
			return (update_existing_node(node, value));
		if (!node->next)
			last = node;
		node = node->next;
	}
	append_new_node(last, key, value);
}
