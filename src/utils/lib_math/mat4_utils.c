/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 05:01:49 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:32:18 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_mat4	mat4_rot(t_vec3 rot)
{
	t_mat4		res;
	t_vec2		x;
	t_vec2		y;
	t_vec2		z;

	sincosf(rot.x, &x.sin, &x.cos);
	sincosf(rot.y, &y.sin, &y.cos);
	sincosf(rot.z, &z.sin, &z.cos);
	res.rows[0] = v4sf(
		y.cos * z.cos,
		z.cos * x.sin * y.sin - x.cos * z.sin,
		x.cos * z.cos * y.sin + x.sin * z.sin,
		0.0f);
	res.rows[1] = v4sf(
		y.cos * z.sin,
		x.cos * z.cos + x.sin * y.sin * z.sin,
		-z.cos * x.sin + x.cos * y.sin * z.sin,
		0.0f);
	res.rows[2] = v4sf(
		-y.sin,
		y.cos * x.sin,
		x.cos * y.cos,
		0.0f);
	res.rows[3] = v4sf(0.0f, 0.0f, 0.0f, 1.0f);
	return (res);
}

t_vec3	mat4_mul_vec3(const t_mat4 *src, t_vec3 vec)
{
	t_vec4		res;

	res.v = vec.v;
	res = mat4_mul_vec4(src, res);
	vec.v = res.v;
	return (vec);
}
