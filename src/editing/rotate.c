/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:50 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:47:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editing.h"

void	obj_rotate(t_context *ctx, float angle)
{
	t_quat		rot_delta;
	t_vec3		axis;

	if (ctx->editor.constraint_axis == AXIS_DEFAULT)
		axis = ctx->scene.cam.forward;
	else if (ctx->editor.constraints == 1)
		axis = ctx->editor.axis_primary;
	else
	{
		if (ctx->editor.axis_primary.z == 0.0f && ctx->editor.axis_secondary.z == 0.0f)
			axis = g_forward;
		else if (ctx->editor.axis_primary.y == 0.0f && ctx->editor.axis_secondary.y == 0.0f)
			axis = g_up;
		else
			axis = g_right;
	}
	rot_delta = quat_from_euler_angle(axis, angle);
	ctx->editor.selected_obj->transform.rot = quat_mul(rot_delta, ctx->editor.selected_obj->transform.rot);
	ctx->editor.selected_obj->transform.rot = quat_normalize(ctx->editor.selected_obj->transform.rot);
}
