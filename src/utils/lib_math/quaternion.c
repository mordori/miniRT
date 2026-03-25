/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:21:14 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 19:21:16 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_quat	quat_from_euler(t_vec3 euler)
{
	t_quat		q;
	t_vec2		x;
	t_vec2		y;
	t_vec2		z;

	euler = vec3_scale(euler, 0.5f);
	sincosf(euler.x, &x.sin, &x.cos);
	sincosf(euler.y, &y.sin, &y.cos);
	sincosf(euler.z, &z.sin, &z.cos);
	q.w = x.cos * y.cos * z.cos + x.sin * y.sin * z.sin;
	q.x = x.sin * y.cos * z.cos - x.cos * y.sin * z.sin;
	q.y = x.cos * y.sin * z.cos + x.sin * y.cos * z.sin;
	q.z = x.cos * y.cos * z.sin - x.sin * y.sin * z.cos;
	return (q);
}

t_quat	quat_from_euler_angle(t_vec3 axis, float angle)
{
	t_quat		q;
	t_vec2		theta;

	sincosf(angle * 0.5f, &theta.sin, &theta.cos);
	axis = vec3_normalize(axis);
	q.xyz = vec3_scale(axis, theta.sin);
	q.w = theta.cos;
	return (q);
}

t_quat	quat_normalize(t_quat q)
{
	float		len_sq;

	len_sq = vec4_dot(q, q);
	if (len_sq > LEN_SQ_EPSILON)
		q = vec4_scale(q, 1.0f / sqrtf(len_sq));
	return (q);
}

t_mat4	quat_to_mat4(t_quat q)
{
	t_mat4		m;
	t_vec3		x;
	t_vec3		yz;
	t_vec3		w;

	m = mat4_identity();
	x = vec3(q.x * q.x, q.x * q.y, q.x * q.z);
	yz = vec3(q.y * q.z, q.y * q.y, q.z * q.z);
	w = vec3(q.w * q.x, q.w * q.y, q.w * q.z);
	m.m[0][0] = 1.0f - 2.0f * (yz.y + yz.z);
	m.m[0][1] = 2.0f * (x.y - w.z);
	m.m[0][2] = 2.0f * (x.z + w.y);
	m.m[1][0] = 2.0f * (x.y + w.z);
	m.m[1][1] = 1.0f - 2.0f * (x.x + yz.z);
	m.m[1][2] = 2.0f * (yz.x - w.x);
	m.m[2][0] = 2.0f * (x.z - w.y);
	m.m[2][1] = 2.0f * (yz.x + w.x);
	m.m[2][2] = 1.0f - 2.0f * (x.x + yz.y);
	return (m);
}

t_quat	quat_mul(t_quat a, t_quat b)
{
	t_quat		res;

	res.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	res.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	res.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	res.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	return (res);
}
