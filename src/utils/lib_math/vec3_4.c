/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 18:35:30 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/03 14:23:28 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_sqrt(const t_vec3 vec)
{
	t_vec3		res;

	res.v = _mm_sqrt_ps(vec.v);
	return (res);
}

t_vec3	vec3_clamp(const t_vec3 vec, const float min, const float max)
{
	t_vec3		res;

	res.v = _mm_max_ps(vec.v, v4sf_n(min));
	res.v = _mm_min_ps(res.v, v4sf_n(max));
	return (res);
}

t_vec3	vec3_clamp01(const t_vec3 vec)
{
	return (vec3_clamp(vec, 0.0f, 1.0f));
}

t_vec3	vec3_clamp_mag(const t_vec3 vec, const float max)
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

t_vec3	vec3_add_n(const t_vec3 vec, const float n)
{
	t_v4sf	n_v;
	t_vec3	res;

	n_v = (t_v4sf){n, n, n, n};
	res.v = vec.v + n_v;
	return (res);
}
