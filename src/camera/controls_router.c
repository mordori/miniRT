/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls_router.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:31 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/30 15:12:08 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "input.h"

static inline bool	set_cam_state(t_context *ctx);

bool	control_camera(t_context *ctx, t_vec2i delta)
{
	const t_cam_state	prev_state = ctx->scene.cam.state;

	if (ctx->editor.mode != EDIT_DEFAULT || !set_cam_state(ctx))
		return (false);
	if (!is_cam_action_active(ctx))
	{
		if (prev_state != CAM_DEFAULT)
			end_cam_action(ctx);
		return (true);
	}
	if (prev_state != CAM_DEFAULT || ctx->scene.cam.state == CAM_DEFAULT)
		apply_cam_action(ctx, delta);
	return (true);
}

void	set_default_view(t_context *ctx)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	cam->init_pos = cam->transform.pos;
	cam->init_rot = cam->transform.rot;
	cam->init_focal_len_mm = cam->focal_len_mm;
	cam->init_focus_dist = cam->focus_dist;
}

bool	reset_camera(t_context *ctx)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	cam->transform.pos = cam->init_pos;
	cam->transform.rot = cam->init_rot;
	cam->focal_len_mm = cam->init_focal_len_mm;
	cam->focus_dist = cam->init_focus_dist;
	cam->f_stop = 16.0f;
	cam->shutter_speed = 1.0f / 100.0f;
	cam->iso = 100.0f;
	cam->distance = fmaxf(vec3_dist(cam->transform.pos, g_zero), 0.01f);
	update_camera(ctx, cam);
	cam->control_forward = cam->forward;
	cam->control_right = cam->right;
	ctx->renderer.cam = *cam;
	cam->yaw = atan2f(cam->forward.x, cam->forward.z);
	cam->pitch = asinf(clampf(cam->forward.y, -1.0f, 1.0f));
	return (true);
}

bool	frame_camera(t_context *ctx)
{
	const t_object	*obj = ctx->editor.selected_obj;
	t_camera		*cam;
	float			max_dim;

	if (!obj)
		return (false);
	cam = &ctx->scene.cam;
	max_dim = \
fmaxf(fmaxf(obj->bounds_dims.x, obj->bounds_dims.y), obj->bounds_dims.z);
	if (ctx->img->width < ctx->img->height)
		max_dim /= ctx->scene.cam.aspect;
	if (obj->type == OBJ_PLANE)
	{
		ctx->scene.cam.distance = 20.0f;
		cam->transform.pos = vec3_sub(obj->transform.pos, \
vec3_scale(cam->forward, cam->distance));
	}
	else
	{
		ctx->scene.cam.distance = \
(max_dim * 0.5f) / tanf(SENSOR_HALF_HEIGHT_MM / cam->focal_len_mm * 0.8f);
		cam->transform.pos = vec3_sub(obj->bounds_center, \
vec3_scale(cam->forward, cam->distance));
	}
	update_camera(ctx, cam);
	return (true);
}

static inline bool	set_cam_state(t_context *ctx)
{
	if (ctx->scene.cam.state != CAM_DEFAULT)
		return (true);
	if (ctx->renderer.mode == SOLID && \
mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT))
	{
		if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			begin_cam_action(ctx, CAM_ORBIT);
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
			begin_cam_action(ctx, CAM_ZOOM);
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
			begin_cam_action(ctx, CAM_PAN);
		else
			return (false);
	}
	else if (ctx->renderer.mode != SOLID)
	{
		if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
			begin_cam_action(ctx, CAM_TURN);
		else if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
			begin_cam_action(ctx, CAM_LOOK);
		else
			return (false);
	}
	else
		return (false);
	return (true);
}
