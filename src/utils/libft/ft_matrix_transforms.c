/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_transforms.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:54:07 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 17:01:26 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_matrix.h"

/**
 * Creates a translation matrix that translates by vector t (x, y, z).
 *
 * [ 1	0	0	tx ]
 * [ 0	1	0	ty ]
 * [ 0	0	1	tz ]
 * [ 0	0	0	1  ]
 *
 * @param s Vec3 position vector.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_translate(t_vec3 t)
{
	t_mat4	matrix;

	matrix = mat4_identity();
	matrix.m[0][3] = t.x;
	matrix.m[1][3] = t.y;
	matrix.m[2][3] = t.z;
	return (matrix);
}

/**
 * Creates a scaling matrix that scales by vector s (x, y, z).
 *
 * [ sx	0	0	0 ]
 * [ 0	sy	0	0 ]
 * [ 0	0	sz	0 ]
 * [ 0	0	0	1 ]
 *
 * @param s Vec3 factor vector.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_scale(t_vec3 s)
{
	t_mat4	matrix;

	matrix = mat4_identity();
	matrix.m[0][0] = s.x;
	matrix.m[1][1] = s.y;
	matrix.m[2][2] = s.z;
	return (matrix);
}

/**
 * Creates a rotation matrix that rotates around the X-axis by angle_rad.
 *
 * [ 1		0		0		0 ]
 * [ 0		cos		-sin	0 ]
 * [ 0		sin		cos		0 ]
 * [ 0		0		0		1 ]
 *
 * @param angle_rad Angle in radians.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_rot_x(float angle_rad)
{
	t_mat4	matrix;
	float	cos;
	float	sin;

	matrix = mat4_identity();
	cos = cosf(angle_rad);
	sin = sinf(angle_rad);
	matrix.m[1][1] = cos;
	matrix.m[1][2] = -sin;
	matrix.m[2][1] = sin;
	matrix.m[2][2] = cos;
	return (matrix);
}

/**
 * Creates a rotation matrix that rotates around the Y-axis by angle_rad.
 *
 * [ cos	0		sin		0 ]
 * [ 0		1		0		0 ]
 * [ -sin	0		cos		0 ]
 * [ 0		0		0		1 ]
 *
 * @param angle_rad Angle in radians.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_rot_y(float angle_rad)
{
	t_mat4	matrix;
	float	cos;
	float	sin;

	matrix = mat4_identity();
	cos = cosf(angle_rad);
	sin = sinf(angle_rad);
	matrix.m[0][0] = cos;
	matrix.m[0][2] = sin;
	matrix.m[2][0] = -sin;
	matrix.m[2][2] = cos;
	return (matrix);
}

/**
 * Creates a rotation matrix that rotates around the Z-axis by angle_rad.
 *
 * [ cos	-sin	0		0 ]
 * [ sin	cos		0		0 ]
 * [ 0		0		1		0 ]
 * [ 0		0		0		1 ]
 *
 * @param angle_rad Angle in radians.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_rot_z(float angle_rad)
{
	t_mat4	matrix;
	float	cos;
	float	sin;

	matrix = mat4_identity();
	cos = cosf(angle_rad);
	sin = sinf(angle_rad);
	matrix.m[0][0] = cos;
	matrix.m[0][1] = -sin;
	matrix.m[1][0] = sin;
	matrix.m[1][1] = cos;
	return (matrix);
}
