/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:34:25 by akkim             #+#    #+#             */
/*   Updated: 2026/03/23 01:42:33 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

int	chk_quote(char q)
{
	if (q == '\'')
		return (1);
	else if (q == '\"')
		return (1);
	else if (q == '`')
		return (1);
	return (0);
}	

// 따옴표를 만나면 바로 검사 함수로 이동
void	check_quote(char **line)
{
	int	i;

	if (!line || !*line)
		return ;
	i = 0;
	while ((*line)[i])
	{
		if (chk_quote((*line)[i]))
			i += skip_quote(line, i, (*line)[i]);
		else
			i++;
	}
}
