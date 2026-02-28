/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:49:48 by akkim             #+#    #+#             */
/*   Updated: 2026/02/20 19:47:52 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_string(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	mini_exit(t_info_env *env, char **code)
{
	write(2, "exit\n", 5);
	if (code[1] == NULL)
		exit(env->exit_code);
	else
	{
		if (is_numeric_string(code[1]))
		{
			if (code[2] != NULL)
			{
				ft_putstr_fd("bash: exit: too many arguments", 2);
				env->exit_code = 1;
				return ;
			}
			exit(ft_atoi(code[1]));
		}
		else
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(code[1], 2);
			ft_putstr_fd(" numeric argument required\n", 2);
			exit(2);
		}
	}
}
