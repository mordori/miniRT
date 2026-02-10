/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:28:02 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/10 19:39:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3		res;
	t_v4sf		yzx;
	t_v4sf		zxy;

	yzx = \
_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3, 0, 2, 1)) * \
_mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3, 1, 0, 2));
	zxy = \
_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3, 1, 0, 2)) * \
_mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3, 0, 2, 1));
	res.v = yzx - zxy;
	return (res);
}

float	vec3_dot(t_vec3 a, t_vec3 b)
{
	t_v4sf		res;

	res = a.v * b.v;
	return (res[0] + res[1] + res[2]);
}

t_vec3	vec3_div(t_vec3 vec, float div)
{
	if (fabsf(div) < LEN_EPSILON)
		return (vec3_n(0.0f));
	return (vec3_scale(vec, 1.0f / div));
}

t_vec3	vec3_normalize(t_vec3 vec)
{
	return (vec3_div(vec, vec3_length(vec)));
}

float	vec3_length(t_vec3 vec)
{
	return (sqrtf(vec3_dot(vec, vec)));
}
