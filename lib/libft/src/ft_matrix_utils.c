/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 05:01:49 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 01:58:57 by myli-pen         ###   ########.fr       */
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
