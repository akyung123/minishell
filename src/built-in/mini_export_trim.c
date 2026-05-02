/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_trim.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:09:52 by sejang            #+#    #+#             */
/*   Updated: 2026/05/02 23:10:26 by sejang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "env.h"

char	*trim_quotes(char *str)
{
	int		len;
	char	*new_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\"' && str[len - 1] == '\"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		new_str = ft_substr(str, 1, len - 2);
		return (new_str);
	}
	return (ft_strdup(str));
}
