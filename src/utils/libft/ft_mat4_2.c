/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat4_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:16:36 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/22 00:59:19 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_matrix.h"

bool	mat4_inverse(t_mat4 m, t_mat4 *out)
{
	int			row;
	int			col;
	float		cofactor;
	float		determinant;

	determinant = mat4_det(m);
	if (fabsf(determinant) < M_EPSILON)
		return (false);
	row = 0;
	while (row < 4)
	{
		col = 0;
		while (col < 4)
		{
			cofactor = mat4_cofactor(m, row, col);
			out->m[col][row] = cofactor / determinant;
			++col;
		}
		++row;
	}
	return (true);
}

t_mat3	mat4_sub(t_mat4	m, int row, int col)
{
	t_mat3		result;
	int			src_row;
	int			src_col;
	int			new_row;
	int			new_col;

	src_row = 0;
	new_row = 0;
	while (src_row < 4)
	{
		if (src_row == row && ++src_row)
			continue ;
		src_col = 0;
		new_col = 0;
		while (src_col < 4)
		{
			if (src_col == col && ++src_col)
				continue ;
			result.m[new_row][new_col++] = m.m[src_row][src_col++];
		}
		++new_row;
		++src_row;
	}
	return (result);
}

float	mat4_minor(t_mat4 m, int row, int col)
{
	return (mat3_det(mat4_sub(m, row, col)));
}

float	mat4_cofactor(t_mat4 m, int row, int col)
{
	float		minor;

	minor = mat4_minor(m, row, col);
	if ((row + col) & 1)
		minor = -minor;
	return (minor);
}

float	mat4_det(t_mat4 m)
{
	float		det;
	int			col;

	det = 0;
	col = 0;
	while (col < 4)
	{
		det += m.m[0][col] * mat4_cofactor(m, 0, col);
		++col;
	}
	return (det);
}
