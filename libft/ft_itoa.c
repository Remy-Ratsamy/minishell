/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:02:00 by gechavia          #+#    #+#             */
/*   Updated: 2025/05/13 13:16:16 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(long n)
{
	int	total;

	total = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		total++;
	}
	while (n > 0)
	{
		n /= 10;
		total++;
	}
	return (total);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		size;
	long	nb;

	nb = (long)n;
	size = ft_intlen(n);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	str[size] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		str[0] = '0';
	while (nb > 0)
	{
		str[--size] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}
