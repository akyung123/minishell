/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_fuction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:12:38 by akkim             #+#    #+#             */
/*   Updated: 2026/01/19 18:12:57 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	num;

	if (n == -2147483648)
	{
		write(fd, "2147483648", 10);
		return ;
	}
	else if (n == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	else if (n < 0)
		n *= -1;
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else
	{
		num = '0' + n;
		write(fd, &num, 1);
	}
}

void	ft_putunbr_fd(unsigned int n, int fd)
{
	char	num;

	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
	else
	{
		num = '0' + n;
		write(fd, &num, 1);
	}
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(s);
	i = 0;
	while (i < len)
		ft_putchar_fd(s[i++], fd);
}
