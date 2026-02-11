/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:49:39 by akkim             #+#    #+#             */
/*   Updated: 2026/02/11 17:14:06 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

void	mini_cd(t_info_env *env, char *path)
{
	char	*prv;
	char	*old;
	t_env	*tmp;

	if (chdir(path) == 0)
	{
		prv = getcwd(NULL, 0);
		tmp = find_env_node(env->head, "OLDPWD");
		old = tmp->value;
		tmp->value = prv;
		free(old);
		env->exit_code = 0;
		return ;
	}
	ft_putstr_fd("bash: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	env->exit_code = 1;
	return ;
}
