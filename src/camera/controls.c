/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:33 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:47:35 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "input.h"

void	cam_look(t_context *ctx, t_vec2i delta)
{
	float		speed;

	speed = ctx->mlx->delta_time * 60.0f * SENS_LOOK * \
(14.0f / ctx->scene.cam.focal_len_mm);
	ctx->scene.cam.yaw += (float)delta.x * speed;
	ctx->scene.cam.pitch -= (float)delta.y * speed;
}

void	cam_turn(t_context *ctx, t_vec2i delta)
{
	cam_look(ctx, delta);
	update_camera(ctx, &ctx->scene.cam);
	ctx->scene.cam.control_forward = ctx->scene.cam.forward;
	ctx->scene.cam.control_right = ctx->scene.cam.right;
}

void	cam_orbit(t_context *ctx, t_vec2i delta)
{
	(void)ctx;
	(void)delta;
}

void	cam_zoom(t_context *ctx, t_vec2i delta)
{
	(void)ctx;
	(void)delta;
}

void	cam_pan(t_context *ctx, t_vec2i delta)
{
	(void)ctx;
	(void)delta;
}
