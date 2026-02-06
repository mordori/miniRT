/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:03:20 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/05 21:24:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_mul(t_vec3 a, t_vec3 b)
{
	t_vec3		res;

	res.v = a.v * b.v;
	return (res);
}

t_vec3	vec3_reflect(t_vec3 vec, t_vec3 n)
{
	return (vec3_sub(vec, vec3_scale(n, 2 * vec3_dot(vec, n))));
}

t_vec3	vec3_unit_random(uint32_t *seed)
{
	t_vec3	result;
	float	len_sq;

	while (true)
	{
		result = vec3(randomfn11(seed), randomfn11(seed), randomfn11(seed));
		len_sq = vec3_dot(result, result);
		if (len_sq <= 1.0f && fabsf(len_sq) >= LEN_SQ_EPSILON)
			return (vec3_div(result, sqrtf(len_sq)));
	}
}

t_vec3	vec3_min(t_vec3 vec, float min)
{
	t_vec3		res;
	t_v4sf		limit;

	limit = v4sf(min, min, min, M_INF);
	res.v = _mm_min_ps(vec.v, limit);
	return (res);
}

t_vec3	vec3_max(t_vec3 vec, float max)
{
	t_vec3		res;
	t_v4sf		limit;

	limit = v4sf(max, max, max, -M_INF);
	res.v = _mm_max_ps(vec.v, limit);
	return (res);
}

// Bitwise selection mask for branchless arithmetic and compiler bitwise reinterpretation
// t_vec3	vec3_min(t_vec3 vec, float min)
// {
// 	t_vec3		res;
// 	t_v4sf		limit;
// 	t_v4si		mask;
// 	t_v4si		vec_int;
// 	t_v4si		limit_int;

// 	limit = v4sf(min, min, min, M_INF);
// 	mask = (vec.v < limit);
// 	vec_int = (t_v4si)vec.v;
// 	limit_int = (t_v4si)limit;
// 	vec_int = (vec_int & mask) | (limit_int & ~mask);
// 	res.v = (t_v4sf)vec_int;
// 	return (res);
// }
