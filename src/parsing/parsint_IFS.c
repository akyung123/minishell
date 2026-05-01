/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsint_IFS.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 23:10:26 by akkim             #+#    #+#             */
/*   Updated: 2026/05/01 23:42:19 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

char	*get_ifs_value(t_info_env *env)
{
	char	*ifs;

	ifs = get_env_val_all(env, "IFS");
	if (!ifs)
		return (" \t\n");
	return (ifs);
}

int	is_ifs_char(char c, char *ifs)
{
	int	i;

	if (!ifs)
		return (0);
	i = 0;
	while (ifs[i])
	{
		if (c == ifs[i])
			return (1);
		i++;
	}
	return (0);
}
