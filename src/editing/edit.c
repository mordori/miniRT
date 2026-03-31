/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:47 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/31 20:02:38 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editing.h"
#include "lights.h"
#include "utils.h"

static inline void	switch_edit_mode(\
t_context *ctx, mlx_key_data_t keydata);

bool	config_editor(t_context *ctx, mlx_key_data_t keydata)
{
	const t_edit_mode	prev_mode = ctx->editor.mode;
	const t_axis		prev_constraints = ctx->editor.constraint_axis;

	if (!ctx->editor.selected_obj || keydata.action != MLX_PRESS)
		return (false);
	switch_edit_mode(ctx, keydata);
	if (ctx->editor.mode != EDIT_DEFAULT)
		set_axis_constraints(ctx, keydata);
	if (ctx->editor.constraint_axis != prev_constraints || \
(prev_mode != EDIT_DEFAULT && ctx->editor.mode != prev_mode))
	{
		ctx->editor.selected_obj->transform = ctx->editor.orig_transform;
		update_transform(&ctx->editor.selected_obj->transform);
		update_bounds(ctx->editor.selected_obj);
		update_light_radius(ctx);
	}
	return (ctx->editor.mode != prev_mode || \
ctx->editor.constraint_axis != prev_constraints);
}

static inline void	switch_edit_mode(\
t_context *ctx, mlx_key_data_t keydata)
{
	t_edit_mode			mode;

	mode = EDIT_DEFAULT;
	if (keydata.key == MLX_KEY_G)
		mode = EDIT_TRANSLATE;
	else if (keydata.key == MLX_KEY_R)
		mode = EDIT_ROTATE;
	else if (keydata.key == MLX_KEY_S)
		mode = EDIT_SCALE;
	if (mode == EDIT_DEFAULT || ctx->editor.mode == mode)
		return ;
	if (ctx->editor.mode == EDIT_DEFAULT)
	{
		ctx->editor.orig_transform = ctx->editor.selected_obj->transform;
		begin_edit_action(ctx, mode);
	}
	else
	{
		ctx->editor.constraint_axis = AXIS_DEFAULT;
		ctx->editor.constraints = 0;
		ctx->editor.axis_primary = ctx->scene.cam.right;
		ctx->editor.axis_secondary = ctx->scene.cam.up;
	}
	ctx->editor.mode = mode;
}

bool	edit_object(t_context *ctx, t_vec2i delta)
{
	static t_edit_mode	prev_mode = EDIT_DEFAULT;
	const t_edit_mode	current_mode = ctx->editor.mode;

	if (ctx->scene.cam.state != CAM_DEFAULT || current_mode == EDIT_DEFAULT || \
!ctx->editor.selected_obj)
	{
		prev_mode = current_mode;
		return (false);
	}
	if (prev_mode == EDIT_DEFAULT && current_mode != EDIT_DEFAULT)
	{
		prev_mode = current_mode;
		return (true);
	}
	edit_action(ctx, delta);
	prev_mode = current_mode;
	return (delta.x != 0 || delta.y != 0);
}

float	eval_speed(t_context *ctx)
{
	float		speed;
	float		dist;

	if (ctx->editor.mode == EDIT_TRANSLATE)
	{
		dist = vec3_dist(ctx->scene.cam.transform.pos, \
ctx->editor.selected_obj->transform.pos);
		dist = fmaxf(dist, 1.0f);
		speed = SENS_TRANSLATE * dist * (14.0f / ctx->scene.cam.focal_len_mm);
	}
	else if (ctx->editor.mode == EDIT_ROTATE)
		speed = SENS_ROTATE;
	else
		speed = SENS_SCALE;
	return (speed);
}

float	eval_magnitude(t_context *ctx, t_vec2i delta, float speed)
{
	float		magnitude;
	float		right_align;
	float		up_align;

	if (ctx->editor.mode == EDIT_SCALE)
		magnitude = eval_scale_magnitude(ctx, delta, speed);
	else
	{
		if (ctx->editor.constraints == 1)
		{
			right_align = \
vec3_dot(ctx->scene.cam.right, ctx->editor.axis_primary);
			up_align = vec3_dot(ctx->scene.cam.up, ctx->editor.axis_primary);
			if (fabsf(right_align) < 0.1f && fabsf(up_align) < 0.1f)
				magnitude = ((float)(delta.x - delta.y)) * speed;
			else
				magnitude = \
((float)delta.x * right_align - (float)delta.y * up_align) * speed;
		}
		else
			magnitude = ((float)(delta.x - delta.y)) * speed;
	}
	return (magnitude);
}
