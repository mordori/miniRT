/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:07:48 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/30 15:11:44 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_quat	quat_from_dir(t_vec3 dir)
{
	const t_vec3	up = {{0.0f, 1.0f, 0.0f}};
	t_vec3			cross;
	t_quat			q;
	float			dot;

	dir = vec3_normalize(dir);
	dot = vec3_dot(up, dir);
	if (dot < -0.999f)
		return (quat_from_euler(vec3(M_PI, 0.0f, 0.0f)));
	cross = vec3_cross(up, dir);
	q.x = cross.x;
	q.y = cross.y;
	q.z = cross.z;
	q.w = 1.0f + dot;
	return (quat_normalize(q));
}
