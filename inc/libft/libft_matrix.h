/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_matrix.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:44:48 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/29 06:43:13 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MATRIX_H
# define LIBFT_MATRIX_H

# include <math.h>

# include "libft_defs.h"
# include "libft_math.h"

struct s_mat2
{
	float	m[2][2];
};

struct s_mat3
{
	float	m[3][3];
};

struct s_mat4
{
	float	m[4][4];
};

float	mat2_det(t_mat2 m);
bool	mat3_inverse(t_mat3 m, t_mat3 *out);
t_mat2	mat3_sub(t_mat3	m, int row, int column);
float	mat3_minor(t_mat3 m, int row, int column);
float	mat3_cofactor(t_mat3 m, int row, int column);
float	mat3_det(t_mat3 m);
t_vec3	mat3_mul_vec3(t_mat3 m, t_vec3 v);
bool	mat4_inverse(t_mat4 m, t_mat4 *out);
t_mat3	mat4_sub(t_mat4	m, int row, int column);
float	mat4_minor(t_mat4 m, int row, int column);
float	mat4_cofactor(t_mat4 m, int row, int column);
float	mat4_det(t_mat4 m);
t_mat4	mat4_identity(void);
t_mat4	mat4_transpose(t_mat4 m);
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
