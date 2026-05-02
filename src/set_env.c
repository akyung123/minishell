/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:26:48 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 23:36:39 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "execution.h"
// env setting funtion for minishell
// linked list
// SHLVL nesting O

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

// add node
void	env_add_back(t_env **head, t_env *new)
{
	t_env	*curr;

	if (!*head)
	{
		*head = new;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

void	init_hidden_vars(t_info_env *env)
{
	static char	*vars[] = {
		"0=minishell",
		"MINISHELL=/home/akkim/Public/42_curse/C3/minishell/minishell",
		"IFS= \t\n", "BASH=/usr/bin/bash", "BASH_VERSION=5.1.16(1)-release",
		"BASHOPTS=checkwinsize:cmdhist:complete_fullquote:expand_aliases",
		"EUID=102092", "HOSTTYPE=x86_64", "MACHTYPE=x86_64-pc-linux-gnu",
		"OSTYPE=linux-gnu", "OPTIND=1", "PS1=minishell$ ", "PS2=> ", "PS4=+ ",
		NULL
	};
	int			i;
	char		*key;
	char		*val;
	char		*eq;

	i = -1;
	while (vars[++i])
	{
		eq = ft_strchr(vars[i], '=');
		key = ft_substr(vars[i], 0, eq - vars[i]);
		val = ft_strdup(eq + 1);
		if (!find_env_node(env->hide_head, key))
			env_add_back(&(env->hide_head), new_env_node(key, val));
		free(key);
		free(val);
	}
}

void	env_null(t_info_env *env)
{
	env->head = NULL;
	env->envp = NULL;
	env->hide_head = NULL;
	env->exit_code = 0;
}

// reset funtion
// use in main
void	init_env(t_info_env *env, char **envp)
{
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	i = 0;
	env_null(env);
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			key = ft_substr(envp[i], 0, (size_t)(eq - envp[i]));
			value = ft_strdup(eq + 1);
			env_add_back(&env->head, new_env_node(key, value));
			free(key);
			free(value);
		}
		i++;
	}
	update_shlvl(env);
	init_hidden_vars(env);
	update_envp_array(env);
}
