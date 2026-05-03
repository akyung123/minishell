/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:49:39 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 19:32:26 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

static char	*resolve_home(t_info_env *env)
{
	t_env	*tmp;

	tmp = find_env_node(env->head, "HOME");
	if (!tmp || !tmp->value)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		env->exit_code = 1;
		return (NULL);
	}
	return (tmp->value);
}

static char	*resolve_oldpwd(t_info_env *env)
{
	t_env	*tmp;

	tmp = find_env_node(env->head, "OLDPWD");
	if (!tmp || !tmp->value)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		env->exit_code = 1;
		return (NULL);
	}
	ft_putendl_fd(tmp->value, 1);
	return (tmp->value);
}

static char	*resolve_target_path(t_info_env *env, char *path)
{
	if (!path)
		return (resolve_home(env));
	if (ft_strcmp(path, "-") == 0)
		return (resolve_oldpwd(env));
	return (path);
}

static void	update_pwd(t_info_env *env)
{
	char	*prv;
	char	*curr;

	prv = get_env_val(env->head, "PWD");
	update_env_val(env->head, "OLDPWD", prv);
	curr = getcwd(NULL, 0);
	update_env_val(env->head, "PWD", curr);
	free(curr);
}

void	mini_cd(t_info_env *env, char *path)
{
	char	*target_path;

	target_path = resolve_target_path(env, path);
	if (!target_path)
		return ;
	if (chdir(target_path) == 0)
	{
		update_pwd(env);
		env->exit_code = 0;
		return ;
	}
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	env->exit_code = 1;
}
