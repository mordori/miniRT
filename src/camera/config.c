/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:28 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:47:29 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

static inline bool	set_f_stop(t_context *ctx);
static inline bool	set_focal_length(t_context *ctx);
static inline bool	set_focus_dist(t_context *ctx);

bool	config_camera(t_context *ctx)
{
	bool		dirty;

	dirty = false;
	if (set_f_stop(ctx))
		dirty = true;
	if (set_focal_length(ctx))
		dirty = true;
	if (set_focus_dist(ctx))
		dirty = true;
	return (dirty);
}

static inline bool	set_f_stop(t_context *ctx)
{
	float		mul;
	float		div;

	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT) && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT) && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT)))
	{
		mul = powf(2.0f, 0.15f * ctx->mlx->delta_time);
		div = 1.0f / mul;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT))
			ctx->scene.cam.f_stop *= div;
		else
			ctx->scene.cam.f_stop *= mul;
		ctx->scene.cam.f_stop = clampf(ctx->scene.cam.f_stop, 0.95f, 128.0f);
		return (true);
	}
	else
		return (false);
}

static inline bool	set_focal_length(t_context *ctx)
{
	float		mul;
	float		div;

	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_L) && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_K)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_K) && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_L)))
	{
		mul = powf(2.0f, 0.5f * ctx->mlx->delta_time);
		div = 1.0f / mul;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_K))
			ctx->scene.cam.focal_len_mm *= div;
		else
			ctx->scene.cam.focal_len_mm *= mul;
		ctx->scene.cam.focal_len_mm = \
clampf(ctx->scene.cam.focal_len_mm, 14.0f, 800.0f);
		return (true);
	}
	else
		return (false);
}

static inline bool	set_focus_dist(t_context *ctx)
{
	float		mul;
	float		div;

	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN) && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_UP)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_UP) && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN)))
	{
		mul = powf(2.0f, 0.3f * ctx->mlx->delta_time);
		div = 1.0f / mul;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN))
			ctx->scene.cam.focus_dist *= div;
		else
			ctx->scene.cam.focus_dist *= mul;
		ctx->scene.cam.focus_dist = \
clampf(ctx->scene.cam.focus_dist, 0.1f, 1000.0f);
		return (true);
	}
	else
		return (false);
}
