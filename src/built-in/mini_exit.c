/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:49:48 by akkim             #+#    #+#             */
/*   Updated: 2026/05/01 22:25:07 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"
#include "parsing.h"

#include <limits.h>

static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

long long	ft_atoll_check(const char *str, int *err)
{
	unsigned long long	res;
	int					sign;
	int					i;

	res = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	if (!str[i])
		return (*err = 1, 0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i++] - '0');
		if ((sign == 1 && res > LLONG_MAX) || \
			(sign == -1 && res > (unsigned long long)LLONG_MAX + 1))
			return (*err = 1, 0);
	}
	if (str[i])
		return (*err = 1, 0);
	return (res * sign);
}

static void	free_exit(t_info_env *env, int code)
{
	free_command_line(env->pipex->cmd_line);
	free_pipex(env->pipex);
	exit(code);
}

void	mini_exit(t_info_env *env, char **code)
{
	int			err;
	long long	exit_code;

	write(2, "exit\n", 5);
	if (code[1] == NULL || !code)
		free_exit(env, env->exit_code);
	err = 0;
	exit_code = ft_atoll_check(code[1], &err);
	if (err)
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(code[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_exit(env, 255);
	}
	if (code[2] != NULL)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		env->exit_code = 1;
		return ;
	}
	free_exit(env, exit_code % 256);
}
