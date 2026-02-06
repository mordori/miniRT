/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 18:35:30 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/05 21:24:24 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_sqrt(t_vec3 vec)
{
	t_vec3		res;

	res.v = _mm_sqrt_ps(vec.v);
	return (res);
}

t_vec3	vec3_clamp(t_vec3 vec, float min, float max)
{
	t_vec3		res;

	res.v = _mm_max_ps(vec.v, v4sf_n(min));
	res.v = _mm_min_ps(res.v, v4sf_n(max));
	return (res);
}

t_vec3	vec3_clamp01(t_vec3 vec)
{
	return (vec3_clamp(vec, 0.0f, 1.0f));
}

t_vec3	vec3_clamp_mag(t_vec3 vec, float max)
{
	float	len_sq;
	float	scale;

	len_sq = vec3_dot(vec, vec);
	if (len_sq > max * max)
	{
		scale = max / sqrtf(len_sq);
		return (vec3_scale(vec, scale));
	}
	return (vec);
}

t_vec3	vec3_add_n(t_vec3 vec, float n)
{
	t_v4sf	n_v;
	t_vec3	res;

	n_v = v4sf_n(n);
	res.v = vec.v + n_v;
	return (res);
}
