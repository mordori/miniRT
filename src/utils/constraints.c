/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constraints.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:20:55 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 21:31:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static inline t_vec3	get_plane(t_context *ctx, t_vec3 axis, t_vec3 n);

void	planar_basis(t_context *ctx, t_vec3 n)
{
	t_vec3		plane_x;
	t_vec3		plane_y;

	plane_x = get_plane(ctx, ctx->scene.cam.right, n);
	plane_y = get_plane(ctx, ctx->scene.cam.up, n);
	if (vec3_dot(plane_x, plane_x) > 0.0001f)
		plane_x = vec3_normalize(plane_x);
	else
		plane_x = g_zero;
	if (vec3_dot(plane_y, plane_y) > 0.0001f)
		plane_y = vec3_normalize(plane_y);
	else
		plane_y = g_zero;
	ctx->editor.axis_primary = plane_x;
	ctx->editor.axis_secondary = plane_y;
}

static inline t_vec3	get_plane(t_context *ctx, t_vec3 axis, t_vec3 n)
{
	t_vec3		plane;
	float		dot;

	dot = vec3_dot(axis, n);
	plane = vec3_sub(axis, vec3_scale(n, dot));
	if (vec3_dot(plane, plane) < 0.001f)
	{
		dot = vec3_dot(ctx->scene.cam.forward, n);
		plane = vec3_sub(ctx->scene.cam.forward, vec3_scale(n, dot));
	}
	return (plane);
}
