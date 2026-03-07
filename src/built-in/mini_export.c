/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:13:55 by akkim             #+#    #+#             */
/*   Updated: 2026/02/10 20:09:38 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "env.h"

static int  valid_identifier(char *key)
{
    int i;

    if (!key || !key[0])
        return (0);
    if (!(ft_isalpha(key[0]) || key[0] == '_'))
        return (0);
    i = 1;
    while (key[i])
    {
        if (!(ft_isalnum(key[i]) || key[i] == '_'))
            return (0);
        i ++;
    }
    return (1);
}

static void print_export_node(t_env *node)
{
    if (node->value)
        ft_printf("declare -x %s=\"%s\"\n", node->key, node->value);
    else
        ft_printf("declare -x %s\n", node->key);
}

static void print_sorted_export(t_info_env *env)
{
    t_env   **arr;
    t_env   *cur;
    int     i;
    int     j;
    int     n;

    n = 0;
    cur = env->head;
    while (cur && ++n)
        cur = cur->next;
    arr = malloc(sizeof(t_env *) * n);
    if (!arr)
        return ;
    cur = env -> head;
    i = 0;
    while (cur)
    {
        arr[i++] = cur;
        cur = cur->next;
    }
    i = 0;
    while (i < n - 1)
    {
        j = 0;
        while (j < n - i - 1)
        {
            if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
            {
                cur = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = cur;
            }
            j ++;
        }
        i ++;
    }
    i = 0;
    while (i < n)
        print_export_node(arr[i++]);
    free(arr);
    env->exit_code = 0;
}

// KEY = VALUE
static void set_env_value (t_info_env *env, char *key, char *value)
{
    t_env *node;

    node = find_env_node(env->head, key);
    if (node)
    {
        if (node->value)
            free(node->value);
        node->value = ft_strdup(value);
    }
    else
        env_add_back(&env->head, new_env_node(key, value));
}

void    mini_export(t_info_env *env, char *str)
{
    char    *eq;
    char    *key;
    char    *value;

    if (!str)
    {
        env->exit_code = 0;
        return (print_sorted_export(env));
    }
    eq = ft_strchr(str, '=');
    if (!eq)
    {
        if (!valid_identifier(str))
            return ;
        if (!find_env_node(env->head, str))
            env_add_back(&env->head, new_env_node(str, NULL));
        env->exit_code = 0;
        return ;
    }
    key = ft_substr(str, 0, eq - str);
    if (!valid_identifier(key))
    {
        free(key);
        return ;
    }
    value = ft_strdup(eq + 1);
    set_env_value(env, key, value);
    free(key);
    free(value);
    env->exit_code = 0;
}
