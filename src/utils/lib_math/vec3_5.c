/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:55:00 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/13 23:07:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_negate(t_vec3 vec)
{
	return (vec3_scale(vec, -1.0f));
}

t_vec3	vec3_lerp(t_vec3 a, t_vec3 b, float t)
{
	t_vec3		res;

	res.v = a.v + (b.v - a.v) * v4sf_n(t);
	return (res);
}

t_vec3	vec3_schlick(t_vec3 f0, float u)
{
	float		f;

	f = powf(1.0f - u, 5.0f);
	return (vec3_add(vec3_scale(f0, (1.0f - f)), vec3_n(f)));
}

t_vec3	vec3_bias(t_vec3 vec, t_vec3 n)
{
	return (vec3_add(vec, vec3_scale(n, B_EPSILON)));
}

uint32_t	vec3_to_uint32(t_vec3 vec)
{
	__m128i			res;

	res = _mm_cvtps_epi32(vec.v * v4sf_n(255.0f));
	res = _mm_packs_epi32(res, res);
	res = _mm_packus_epi16(res, res);
	return ((uint32_t)_mm_cvtsi128_si32(res) | 0xFF000000);
}
