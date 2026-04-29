/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:59:15 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 08:06:15 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_option(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	mini_echo(t_info_env *env, char **str)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	(void)env;
	while (str[i] && is_n_option(str[i]))
	{
		n_flag = 1;
		i++;
	}
	while (str[i])
	{
		ft_putstr_fd(str[i], 1);
		if (str[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (n_flag == 0)
		write(1, "\n", 1);
	env->exit_code = 0;
}
