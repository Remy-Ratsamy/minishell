/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 15:00:32 by gechavia          #+#    #+#             */
/*   Updated: 2025/09/23 10:38:30 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	long	result;
	int		digit;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i++] - '0';
		if (sign == 1 && (result > (LONG_MAX - digit) / 10))
			return (LONG_MAX);
		if (sign == -1 && digit && (result > (-(LONG_MIN + digit)) / 10))
			return (LONG_MIN);
		result = result * 10 + digit;
	}
	return (result * sign);
}
