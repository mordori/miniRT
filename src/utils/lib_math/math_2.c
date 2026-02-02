/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:39:37 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 12:13:53 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

float	ft_normalize(const float value, const float min, const float max)
{
	return (ft_clamp01((value - min) / (max - min)));
}

float	ft_clamp01(const float value)
{
	return (ft_clamp(value, 0.0f, 1.0f));
}

float	ft_clamp(float value, const float min, const float max)
{
	value = fminf(value, max);
	value = fmaxf(value, min);
	return (value);
}

float	degrees_to_rad(const float degrees)
{
	return (degrees * M_PI / 180.0f);
}

uint32_t	ft_uint_min(const uint32_t a, const uint32_t b)
{
	if (a < b)
		return (a);
	return (b);
}
