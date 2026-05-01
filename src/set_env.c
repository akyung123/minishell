/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:26:48 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 02:52:54 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "execution.h"
// env setting funtion for minishell
// linked list
// SHLVL nesting O

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

void	update_envp_array(t_info_env *env)
{
	int		count;
	int		i;
	char	**new_envp;
	t_env	*curr;
	char	*tmp;

	count = count_env_nodes(env->head);
	new_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return ;
	curr = env->head;
	i = 0;
	while (curr)
	{
		if (curr->value)
		{
			tmp = ft_strjoin(curr->key, "=");
			new_envp[i] = ft_strjoin(tmp, curr->value);
			free(tmp);
		}
		else
			new_envp[i] = ft_strdup(curr->key);
		curr = curr->next;
		i++;
	}
	new_envp[i] = NULL;
	if (env->envp)
		free_split(env->envp);
	env->envp = new_envp;
}

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

// reset funtion
// use in main
void	init_env(t_info_env *env, char **envp)
{
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	i = 0;
	env->head = NULL;
	env->envp = NULL;
	env->hide_head = NULL;
	env->exit_code = 0;
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

// all free
void	all_free_env(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->key)
			free(head->key);
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}
