/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:08:49 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:16:45 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "execution.h"

void	update_shlvl(t_info_env *env)
{
	t_env	*node;
	int		shlvl;

	node = find_env_node(env->head, "SHLVL");
	if (!node)
	{
		env_add_back(&(env->head), new_env_node("SHLVL", "1"));
		return ;
	}
	shlvl = ft_atoi(node->value) + 1;
	if (shlvl >= 1000)
	{
		ft_putstr_fd("bash: warning: shell level (", 2);
		ft_putnbr_fd(shlvl, 2);
		ft_putendl_fd(") too high, resetting to 1", 2);
		shlvl = 1;
	}
	else if (shlvl < 0)
		shlvl = 0;
	free(node->value);
	node->value = ft_itoa(shlvl);
}

void	update_last_arg(t_info_env *env, char *last_arg)
{
	t_env	*node;

	if (!last_arg)
		return ;
	node = env->head;
	while (node)
	{
		if (ft_strcmp(node->key, "_") == 0)
		{
			free(node->value);
			node->value = ft_strdup(last_arg);
			update_envp_array(env);
			return ;
		}
		node = node->next;
	}
	env_add_back(&env->head, new_env_node("_", last_arg));
	update_envp_array(env);
}

static int	count_env_nodes(t_env *head)
{
	int	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	*ft_new_envp(t_env *curr)
{
	char	*tmp;
	char	*new_envp;

	if (curr->value)
	{
		tmp = ft_strjoin(curr->key, "=");
		new_envp = ft_strjoin(tmp, curr->value);
		free(tmp);
	}
	else
		new_envp = ft_strdup(curr->key);
	return (new_envp);
}

void	update_envp_array(t_info_env *env)
{
	int		count;
	int		i;
	char	**new_envp;
	t_env	*curr;

	count = count_env_nodes(env->head);
	new_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return ;
	curr = env->head;
	i = 0;
	while (curr)
	{
		new_envp[i] = ft_new_envp(curr);
		curr = curr->next;
		i++;
	}
	new_envp[i] = NULL;
	if (env->envp)
		free_split(env->envp);
	env->envp = new_envp;
}
