/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/02/07 16:25:22 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// find path
static char	**find_path(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

static char	*find_user(char **envp)
{
	char	*user;
	int 	i;
	int		j;

	i = 0;
	while (ft_strncmp(envp[i], "USER=", 5) != 0)
		i++;
	user = malloc(ft_strlen(envp[i]));
	j = 0;
	while (envp[i][j] != '\0')
	{
		user[j] = envp[i][j + 5];
		j++;
	}
	return (user);
}

// env 파싱 파트
void	set_env(t_env *env, char **envp)
{
	env->path = find_path(envp);
	env->user = find_user(envp);
	env->pwd  = find_pwd(envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = malloc(sizeof(t_env));
	set_env(env, envp);
	ft_printf("%s", env->path[0]);
	ft_printf("%s", env->user);
	mini_echo("hello\n", env);
}
