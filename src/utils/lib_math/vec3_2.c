/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:28:02 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 10:50:10 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_cross(const t_vec3 a, const t_vec3 b)
{
	static const t_v4si	mask_yzx = {1, 2, 0, 3};
	static const t_v4si	mask_zxy = {2, 0, 1, 3};
	t_vec3				res;
	t_v4sf				a_yzx_b_zxy;
	t_v4sf				a_zxy_b_yzx;

	a_yzx_b_zxy = __builtin_shuffle(a.v, mask_yzx) * __builtin_shuffle(b.v, mask_zxy);
	a_zxy_b_yzx = __builtin_shuffle(a.v, mask_zxy) * __builtin_shuffle(b.v, mask_yzx);
	res.v = a_yzx_b_zxy - a_zxy_b_yzx;
	return (res);
}

float	vec3_dot(const t_vec3 a, const t_vec3 b)
{
	t_v4sf		res;

	res = a.v * b.v;
	return (res[0] + res[1] + res[2] + res[3]);
}

t_vec3	vec3_div(const t_vec3 vec, const float div)
{
	if (fabsf(div) < LEN_EPSILON)
		return (vec3_n(0.0f));
	return (vec3_scale(vec, 1.0f / div));
}

t_vec3	vec3_normalize(const t_vec3 vec)
{
	return (vec3_div(vec, vec3_length(vec)));
}

float	vec3_length(const t_vec3 vec)
{
	return (sqrtf(vec3_dot(vec, vec)));
}
