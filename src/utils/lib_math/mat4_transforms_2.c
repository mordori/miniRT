/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_transforms_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 07:34:25 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/02 02:28:32 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_mat4	mat4_shear(const t_vec3 x, const t_vec3 y, const t_vec3 z)
{
	t_mat4		res;

	res.rows[0] = (t_v4sf){1.0f, x.y, x.z, 0.0f};
	res.rows[1] = (t_v4sf){y.x, 1.0f, y.z, 0.0f};
	res.rows[2] = (t_v4sf){z.x, z.y, 1.0f, 0.0f};
	res.rows[3] = (t_v4sf){0.0f, 0.0f, 0.0f, 1.0f};
	return (res);
}
