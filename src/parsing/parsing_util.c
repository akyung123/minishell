/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:27:34 by akkim             #+#    #+#             */
/*   Updated: 2026/02/27 15:28:15 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int	h;
	int	n;

	if (needle[0] == '\0')
		return ((char *)haystack);
	h = 0;
	while (haystack[h] != '\0')
	{
		n = 0;
		while (haystack[h + n] == needle[n] && haystack[h + n] != '\0')
		{
			n++;
			if (needle[n] == '\0')
				return ((char *)&haystack[h]);
		}
		h++;
	}
	return (0);
}

char	*ft_strrchr(const char *str, int ch)
{
	char	*last;
	int		i;

	last = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == (unsigned char)ch)
		{
			last = (char *)&str[i];
		}
		i++;
	}
	if ((unsigned char)ch == '\0')
	{
		return ((char *)&str[i]);
	}
	return (last);
}