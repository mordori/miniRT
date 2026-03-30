/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:48:00 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:48:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editing.h"

void	obj_translate(\
t_context *ctx, float magnitude, t_vec2i delta, float speed)
{
	t_object		*obj;
	t_vec3			axis;
	t_vec3			half_bounds;
	t_vec3			limit_min;
	t_vec3			limit_max;

	obj = ctx->editor.selected_obj;
	if (ctx->editor.constraints == 1)
		axis = vec3_scale(ctx->editor.axis_primary, magnitude);
	else
	{
		axis = vec3_add(\
vec3_scale(ctx->editor.axis_primary, delta.x * speed), \
vec3_scale(ctx->editor.axis_secondary, -delta.y * speed));
	}
	obj->transform.pos = \
vec3_add(obj->transform.pos, axis);
	half_bounds = vec3_scale(obj->bounds, 0.5f);
	limit_max = vec3_sub(g_world_limit, half_bounds);
	limit_min = vec3_add(g_world_limit_neg, half_bounds);
	limit_min.v = _mm_min_ps(limit_min.v, limit_max.v);
	obj->transform.pos.v = _mm_max_ps(obj->transform.pos.v, limit_min.v);
	obj->transform.pos.v = _mm_min_ps(obj->transform.pos.v, limit_max.v);
}
