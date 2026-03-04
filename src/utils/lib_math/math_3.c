/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:01:29 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/23 00:31:15 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

float	ft_lerp_fast(float a, float b, float t)
{
	return (a + (b - a) * t);
}

float	f_schlick(float u, float f0, float f90)
{
	return (f0 + (f90 - f0) * powf(1.0f - u, 5.0f));
}

// not very effective
// float	f_schlick(float u, float f0, float f90)
// {
// 	float	t;
// 	float	t2;

// 	t = 1.0f - u;
// 	t2 = t * t;
// 	return (f0 + (f90 - f0) * t2 * t2 * t);
// }

float	degrees_to_rad(float degrees)
{
	return (degrees * M_PI / 180.0f);
}

float	rad_to_degrees(float rad)
{
	return (rad * 180.0f * M_1_PI);
}

bool	is_nan_inf(float x)
{
	t_f_ui		n;

	n.f = x;
	return ((n.i & 0x7F800000) == 0x7F800000);
}
