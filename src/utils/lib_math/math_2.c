/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:39:37 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/15 19:40:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

float	ft_normalize(float value, float min, float max)
{
	return (clampf01((value - min) / (max - min)));
}

float	clampf01(float value)
{
	return (clampf(value, 0.0f, 1.0f));
}

float	clampf(float value, float min, float max)
{
	value = fminf(value, max);
	value = fmaxf(value, min);
	return (value);
}

float	clampfn11(float value)
{
	return (clampf(value, -1.0f, 1.0f));
}

uint32_t	ft_uint_min(uint32_t a, uint32_t b)
{
	if (a < b)
		return (a);
	return (b);
}
