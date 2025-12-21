/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:44:24 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/22 00:16:05 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_matrix.h"

/**
 * Identity 4x4 matrix with 1 on the main diagonal and 0 elsewhere.
 *
 * @return Identity 4x4 matrix.
 */
t_mat4	mat4_identity(void)
{
	t_mat4	matrix;

	matrix = (t_mat4){0};
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return (matrix);
}

t_mat4	mat4_transpose(t_mat4 m)
{
	t_mat4	result;
	int		i;
	int		j;

	result = (t_mat4){0};
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result.m[i][j] = m.m[j][i];
			++j;
		}
		++i;
	}
	return (result);
}

/**
 * Multiplies two 4x4 matrices. The result is constructed by multiplying all
 * elements of the row of `a` with the corresponding elements of the
 * column of matrix `b` and then adding these products.
 *
 * @param a 4x4 matrix.
 * @param b 4x4 matrix.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_mul(t_mat4 a, t_mat4 b)
{
	t_mat4	result;
	int		i;
	int		j;
	int		k;

	result = (t_mat4){0};
	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			while (k < 4)
			{
				result.m[i][j] += a.m[i][k] * b.m[k][j];
				++k;
			}
			++j;
		}
		++i;
	}
	return (result);
}

/**
 * Multiplies a 4x4 matrix with a vec4. The vector is 1D array that has the
 * same amount of columns as the matrix has rows, so the concept is the
 * same as multiplying two matrices.
 *
 * @param m 4x4 matrix.
 * @param v Vec4.
 * @return Result of the matrix applied to a vec4.
 */
t_vec4	mat4_mul_vec4(t_mat4 m, t_vec4 v)
{
	t_vec4	result;

	result.x = v.x * m.m[0][0] + v.y * m.m[0][1];
	result.x += v.z * m.m[0][2] + v.w * m.m[0][3];
	result.y = v.x * m.m[1][0] + v.y * m.m[1][1];
	result.y += v.z * m.m[1][2] + v.w * m.m[1][3];
	result.z = v.x * m.m[2][0] + v.y * m.m[2][1];
	result.z += v.z * m.m[2][2] + v.w * m.m[2][3];
	result.w = v.x * m.m[3][0] + v.y * m.m[3][1];
	result.w += v.z * m.m[3][2] + v.w * m.m[3][3];
	return (result);
}
