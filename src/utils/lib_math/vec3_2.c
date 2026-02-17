/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:28:02 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/16 22:28:00 by myli-pen         ###   ########.fr       */
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

t_vec3	vec3_div(t_vec3 vec, float den)
{
	if (fabsf(den) < LEN_EPSILON)
		return (vec3_n(0.0f));
	return (vec3_scale(vec, 1.0f / den));
}

t_vec3	vec3_normalize(t_vec3 vec)
{
	float		len2;
	float		len_recip;

	len2 = vec3_dot(vec, vec);
	if (len2 < LEN_SQ_EPSILON)
		return (vec3_n(0.0f));
	len_recip = 1.0f / sqrtf(len2);
	return (vec3_scale(vec, len_recip));
}

float	vec3_length(t_vec3 vec)
{
	return (sqrtf(vec3_dot(vec, vec)));
}
