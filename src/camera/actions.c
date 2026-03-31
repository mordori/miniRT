/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:22 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/31 18:48:09 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

void	apply_cam_action(t_context *ctx, t_vec2i delta)
{
	t_camera	*cam;

	cam = &ctx->scene.cam;
	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (cam->state == CAM_TURN)
			cam_turn(ctx, delta);
		else if (cam->state == CAM_LOOK)
			cam_look(ctx, delta);
		else if (cam->state == CAM_ORBIT)
			cam_orbit(ctx, delta);
		else if (cam->state == CAM_ZOOM)
			cam_zoom(ctx, delta);
		else if (cam->state == CAM_PAN)
			cam_pan(ctx, delta);
	}
}

void	begin_cam_action(t_context *ctx, t_cam_state state)
{
	ctx->scene.cam.state = state;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos_orig.x, &ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

void	end_cam_action(t_context *ctx)
{
	ctx->scene.cam.state = CAM_DEFAULT;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
	mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos_orig.x, ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

bool	is_cam_action_active(const t_context *ctx)
{
	const t_camera	*cam = &ctx->scene.cam;
	bool			is_held;

	is_held = false;
	if (cam->state == CAM_ORBIT && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
		is_held = true;
	else if (cam->state == CAM_ZOOM && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
		is_held = true;
	else if (cam->state == CAM_PAN && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
		is_held = true;
	else if (cam->state == CAM_TURN && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
		is_held = true;
	else if (cam->state == CAM_LOOK && \
mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
		is_held = true;
	return (is_held);
}
