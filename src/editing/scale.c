/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scale.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/31 19:08:50 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editing.h"
#include "utils.h"

static inline float	get_local_axis_component(\
t_context *ctx, float magnitude, const t_mat4 *m, uint32_t i);

void	obj_scale(t_context *ctx, float magnitude)
{
	t_object	*obj;
	t_mat4		*m;
	t_vec3		axis;
	t_vec3		prev_scale;

	obj = ctx->editor.selected_obj;
	prev_scale = obj->transform.scale;
	if (ctx->editor.constraint_axis == AXIS_DEFAULT)
		axis = vec3_n(expf(magnitude));
	else
	{
		m = &obj->transform.object_to_world;
		axis.x = get_local_axis_component(ctx, magnitude, m, 0);
		axis.y = get_local_axis_component(ctx, magnitude, m, 1);
		axis.z = get_local_axis_component(ctx, magnitude, m, 2);
	}
	obj->transform.scale = vec3_mul(obj->transform.scale, axis);
	obj->transform.scale.v = \
_mm_max_ps(obj->transform.scale.v, vec4_3(vec3_n(0.01f), 1.0f).v);
	update_transform(&obj->transform);
	update_bounds(obj);
	if (get_max_bounds_dim(obj) > WORLD_LIMIT)
		obj->transform.scale = prev_scale;
}

float	eval_scale_magnitude(t_context *ctx, t_vec2i delta, float speed)
{
	t_vec2		obj_screen_pos;
	t_vec2		m_prev;
	t_vec2		m_curr;
	float		dist_prev;
	float		dist_curr;

	obj_screen_pos = world_to_screen(ctx, &ctx->scene.cam, \
&ctx->scene.cam.viewport, ctx->editor.selected_obj->transform.pos);
	m_prev = vec2((float)ctx->mouse.pos_prev.x, (float)ctx->mouse.pos_prev.y);
	m_curr = vec2(m_prev.x + (float)delta.x, m_prev.y + (float)delta.y);
	dist_prev = sqrtf(\
(m_prev.x - obj_screen_pos.x) * (m_prev.x - obj_screen_pos.x) + \
(m_prev.y - obj_screen_pos.y) * (m_prev.y - obj_screen_pos.y));
	dist_curr = sqrtf(\
(m_curr.x - obj_screen_pos.x) * (m_curr.x - obj_screen_pos.x) + \
(m_curr.y - obj_screen_pos.y) * (m_curr.y - obj_screen_pos.y));
	return ((dist_curr - dist_prev) * speed);
}

static inline float	get_local_axis_component(\
t_context *ctx, float magnitude, const t_mat4 *m, uint32_t i)
{
	t_vec3		local;
	t_vec2		w;
	float		res;

	local = vec3_normalize(vec3(m->m[0][i], m->m[1][i], m->m[2][i]));
	w.u = vec3_dot(ctx->editor.axis_primary, local);
	w.v = vec3_dot(ctx->editor.axis_secondary, local);
	res = expf(sqrtf(w.u * w.u + w.v * w.v) * magnitude);
	return (res);
}
