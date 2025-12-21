/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat4_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 05:01:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/22 00:16:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_matrix.h"

/**
 * Multiplies 4x4 rotation matrices in the order of XYZ.
 *
 * @param rot Rotation component of a transform.
 * @return 4x4 matrix product.
 */
t_mat4	mat4_rot(t_vec3 rot)
{
	t_mat4	rx;
	t_mat4	ry;
	t_mat4	rz;

	rx = mat4_rot_x(rot.x);
	ry = mat4_rot_y(rot.y);
	rz = mat4_rot_z(rot.z);
	return (mat4_mul(mat4_mul(rz, ry), rx));
}

/**
 * Multiplies a 4x4 matrix with a temporary vec4. Discards the last element.
 *
 * @param m 4x4 matrix.
 * @param v Vec3.
 * @return Result of the matrix applied to a vec3.
 */
t_vec3	mat4_mul_vec3(t_mat4 model, t_vec3 v)
{
	t_vec4	vec;

	vec = mat4_mul_vec4(model, vec4_3(v, 0.0f));
	return (vec.xyz);
}
