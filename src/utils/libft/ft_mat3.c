/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:15:46 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/22 01:00:50 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_matrix.h"

bool	mat3_inverse(t_mat3 m, t_mat3 *out)
{
	int			row;
	int			col;
	float		cofactor;
	float		determinant;

	determinant = mat3_det(m);
	if (fabsf(determinant) < M_EPSILON)
		return (false);
	row = 0;
	while (row < 3)
	{
		col = 0;
		while (col < 3)
		{
			cofactor = mat3_cofactor(m, row, col);
			out->m[col][row] = cofactor / determinant;
			++col;
		}
		++row;
	}
	return (true);
}

t_mat2	mat3_sub(t_mat3	m, int row, int col)
{
	t_mat2		result;
	int			src_row;
	int			src_col;
	int			new_row;
	int			new_col;

	src_row = 0;
	new_row = 0;
	while (src_row < 3)
	{
		if (src_row == row && ++src_row)
			continue ;
		src_col = 0;
		new_col = 0;
		while (src_col < 3)
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

float	mat3_minor(t_mat3 m, int row, int col)
{
	return (mat2_det(mat3_sub(m, row, col)));
}

float	mat3_cofactor(t_mat3 m, int row, int col)
{
	float		minor;

	minor = mat3_minor(m, row, col);
	if ((row + col) & 1)
		minor = -minor;
	return (minor);
}

float	mat3_det(t_mat3 m)
{
	float		det;
	int			col;

	det = 0;
	col = 0;
	while (col < 3)
	{
		det += m.m[0][col] * mat3_cofactor(m, 0, col);
		++col;
	}
	return (det);
}
