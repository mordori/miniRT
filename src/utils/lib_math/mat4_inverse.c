/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_inverse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:16:36 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:28:01 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

bool	mat4_inverse(const t_mat4 *src, t_mat4 *out)
{
	int			row;
	int			col;
	float		cofactor;
	float		det;
	float		inv_det;

	det = mat4_det(src);
	if (fabsf(det) < M_EPSILON)
		return (false);
	inv_det = 1.0f / det;
	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			cofactor = mat4_cofactor(src, row, col);
			out->m[col][row] = cofactor * inv_det;
			++col;
		}
		++row;
	}
	return (true);
}

t_mat3	mat4_sub(const t_mat4 *src, int skip_r, int skip_c)
{
	t_mat3		res;
	int			row;
	int			col;
	int			r;
	int			c;

	row = 0;
	r = 0;
	while (row < 4)
	{
		if (row == skip_r && ++row)
			continue ;
		c = 0;
		col = 0;
		while (col < 4)
		{
			if (col == skip_c && ++col)
				continue ;
			res.m[r][c++] = src->m[row][col++];
		}
		++r;
		++row;
	}
	return (res);
}

float	mat4_cofactor(const t_mat4 *src, int row, int col)
{
	float		det;
	t_mat3		sub;

	sub = mat4_sub(src, row, col);
	det = mat3_det(&sub);
	if ((row + col) & 1)
		return (-det);
	else
		return (det);
}

float	mat4_det(const t_mat4 *src)
{
	float		res;
	int			i;

	res = 0.0f;
	i = 0;
	while (i < 4)
	{
		res += src->m[0][i] * mat4_cofactor(src, 0, i);
		++i;
	}
	return (res);
}

float	mat3_det(const t_mat3 *src)
{
	float		res;

	res = src->m[0][0] * (src->m[1][1] * src->m[2][2] - src->m[2][1] * src->m[1][2]);
	res -= src->m[0][1] * (src->m[1][0] * src->m[2][2] - src->m[2][0] * src->m[1][2]);
	res += src->m[0][2] * (src->m[1][0] * src->m[2][1] - src->m[2][0] * src->m[1][1]);
	return (res);
}
