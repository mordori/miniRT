/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:44:24 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:28:55 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_mat4	mat4_identity(void)
{
	static const t_v4sf	r0 = {1.0f, 0.0f, 0.0f, 0.0f};
	static const t_v4sf	r1 = {0.0f, 1.0f, 0.0f, 0.0f};
	static const t_v4sf	r2 = {0.0f, 0.0f, 1.0f, 0.0f};
	static const t_v4sf	r3 = {0.0f, 0.0f, 0.0f, 1.0f};
	t_mat4				res;

	res.rows[0] = r0;
	res.rows[1] = r1;
	res.rows[2] = r2;
	res.rows[3] = r3;
	return (res);
}

t_mat4	mat4_transpose(const t_mat4 *src)
{
	t_mat4		res;
	__m128		c0;
	__m128		c1;
	__m128		c2;
	__m128		c3;

	c0 = (__m128)src->rows[0];
	c1 = (__m128)src->rows[1];
	c2 = (__m128)src->rows[2];
	c3 = (__m128)src->rows[3];
	_MM_TRANSPOSE4_PS(c0, c1, c2, c3);
	res.rows[0] = (t_v4sf)c0;
	res.rows[1] = (t_v4sf)c1;
	res.rows[2] = (t_v4sf)c2;
	res.rows[3] = (t_v4sf)c3;
	return (res);
}

t_mat4	mat4_mul(const t_mat4 *a, const t_mat4 *b)
{
	t_mat4		res;
	t_v4sf		a_row;
	int			i;

	i = 0;
	while (i < 4)
	{
		a_row = a->rows[i];
		res.rows[i] = v4sf_n(a_row[0]) * b->rows[0];
		res.rows[i] += v4sf_n(a_row[1]) * b->rows[1];
		res.rows[i] += v4sf_n(a_row[2]) * b->rows[2];
		res.rows[i] += v4sf_n(a_row[3]) * b->rows[3];
		++i;
	}
	return (res);
}

t_vec4	mat4_mul_vec4(const t_mat4 *src, t_vec4 vec)
{
	t_vec4		res;
	__m128		c0;
	__m128		c1;
	__m128		c2;
	__m128		c3;

	c0 = (__m128)src->rows[0];
	c1 = (__m128)src->rows[1];
	c2 = (__m128)src->rows[2];
	c3 = (__m128)src->rows[3];
	_MM_TRANSPOSE4_PS(c0, c1, c2, c3);
	res.v = (t_v4sf)c0 * v4sf_n(vec.x)
			+ (t_v4sf)c1 * v4sf_n(vec.y)
			+ (t_v4sf)c2 * v4sf_n(vec.z)
			+ (t_v4sf)c3 * v4sf_n(vec.w);
	return (res);
}
