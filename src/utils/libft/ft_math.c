/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:14:46 by myli-pen          #+#    #+#             */
/*   Updated: 2025/10/05 18:13:11 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

float	ft_lerp(float a, float b, float t)
{
	return ((1.0f - t) * a + t * b);
}

int	ft_imax(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	ft_imin(int a, int b)
{
	if (a > b)
		return (b);
	return (a);
}

size_t	ft_pow(size_t n, size_t e)
{
	size_t	i;
	size_t	result;

	result = 1;
	i = 0;
	while (i < e)
	{
		result *= n;
		++i;
	}
	return (result);
}

bool	ft_is_pot(size_t n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}
