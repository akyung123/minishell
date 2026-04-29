/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:08:15 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 02:20:37 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void	print_export_node(t_env *node)
{
	if (node->value)
		ft_printf("declare -x %s=\"%s\"\n", node->key, node->value);
	else
		ft_printf("declare -x %s\n", node->key);
}

static int	count_env(t_env *env)
{
	int		n;
	t_env	*cur;

	n = 0;
	cur = env;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}

static t_env	**to_array(t_env *env, int n)
{
	t_env	**arr;
	int		i;

	arr = malloc(sizeof(t_env *) * n);
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	return (arr);
}

static void	sort_array(t_env **arr, int n)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_export(t_info_env *env)
{
	t_env	**arr;
	int		n;
	int		i;

	n = count_env(env->head);
	arr = to_array(env->head, n);
	if (!arr)
		return ;
	sort_array(arr, n);
	i = 0;
	while (i < n)
	{
		print_export_node(arr[i]);
		i++;
	}
	free(arr);
	env->exit_code = 0;
}
