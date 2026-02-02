/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 01:14:46 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 12:14:32 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

float	ft_lerp(const float a, const float b, const float t)
{
	return ((1.0f - t) * a + t * b);
}

int	ft_imax(const int a, const int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	ft_imin(const int a, const int b)
{
	if (a > b)
		return (b);
	return (a);
}

size_t	ft_pow(const size_t n, const size_t e)
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

bool	ft_is_pot(const size_t n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}
