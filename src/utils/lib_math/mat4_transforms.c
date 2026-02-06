/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_transforms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:54:07 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:28:35 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_mat4	mat4_translate(t_vec3 t)
{
	t_mat4		res;

	res.rows[0] = (t_v4sf){1.0f, 0.0f, 0.0f, t.x};
	res.rows[1] = (t_v4sf){0.0f, 1.0f, 0.0f, t.y};
	res.rows[2] = (t_v4sf){0.0f, 0.0f, 1.0f, t.z};
	res.rows[3] = (t_v4sf){0.0f, 0.0f, 0.0f, 1.0f};
	return (res);
}

t_mat4	mat4_scale(t_vec3 s)
{
	t_mat4		res;

	res.rows[0] = (t_v4sf){s.x, 0.0f, 0.0f, 0.0f};
	res.rows[1] = (t_v4sf){0.0f, s.y, 0.0f, 0.0f};
	res.rows[2] = (t_v4sf){0.0f, 0.0f, s.z, 0.0f};
	res.rows[3] = (t_v4sf){0.0f, 0.0f, 0.0f, 1.0f};
	return (res);
}

t_mat4	mat4_rot_x(float rad)
{
	t_mat4	res;
	float	c;
	float	s;

	sincosf(rad, &s, &c);
	res.rows[0] = (t_v4sf){1.0f, 0.0f, 0.0f, 0.0f};
	res.rows[1] = (t_v4sf){0.0f, c, -s, 0.0f};
	res.rows[2] = (t_v4sf){0.0f, s, c, 0.0f};
	res.rows[3] = (t_v4sf){0.0f, 0.0f, 0.0f, 1.0f};
	return (res);
}

t_mat4	mat4_rot_y(float rad)
{
	t_mat4	res;
	float	c;
	float	s;

	sincosf(rad, &s, &c);
	res.rows[0] = (t_v4sf){c, 0.0f, s, 0.0f};
	res.rows[1] = (t_v4sf){0.0f, 1.0f, 0.0f, 0.0f};
	res.rows[2] = (t_v4sf){-s, 0.0f, c, 0.0f};
	res.rows[3] = (t_v4sf){0.0f, 0.0f, 0.0f, 1.0f};
	return (res);
}

t_mat4	mat4_rot_z(float rad)
{
	t_mat4	res;
	float	c;
	float	s;

	sincosf(rad, &s, &c);
	res.rows[0] = (t_v4sf){c, -s, 0.0f, 0.0f};
	res.rows[1] = (t_v4sf){s, c, 0.0f, 0.0f};
	res.rows[2] = (t_v4sf){0.0f, 0.0f, 1.0f, 0.0f};
	res.rows[3] = (t_v4sf){0.0f, 0.0f, 0.0f, 1.0f};
	return (res);
}
