/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_matrix.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:44:48 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 03:18:58 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MATRIX_H
# define LIBFT_MATRIX_H

# include <math.h>

# include "libft_defs.h"
# include "libft_math.h"

struct s_mat4
{
	float	m[4][4];
};

t_mat4	mat4_identity(void);
t_mat4	mat4_mul(t_mat4 a, t_mat4 b);
t_vec4	mat4_mul_vec4(t_mat4 m, t_vec4 v);
t_mat4	mat4_translate(t_vec3 t);
t_mat4	mat4_scale(t_vec3 s);
t_mat4	mat4_rot_x(float angle);
t_mat4	mat4_rot_y(float angle);
t_mat4	mat4_rot_z(float angle);
t_vec3	mat4_mul_vec3(t_mat4 model, t_vec3 v);
t_mat4	mat4_rot(t_vec3 rot);

#endif
