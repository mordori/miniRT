#include "input.h"


bool	is_key_down(t_context *ctx)
{
	bool		dirty;
	float		mul;
	float		div;

	dirty = false;
	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT) && !mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT) && !mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT)))
	{
		mul = powf(2.0f, 0.15f * ctx->mlx->delta_time);
		div = 1.0f / mul;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT))
			ctx->scene.cam.f_stop *= div;
		else
			ctx->scene.cam.f_stop *= mul;
		ctx->scene.cam.f_stop = clampf(ctx->scene.cam.f_stop, 0.95f, 128.0f);
		dirty = true;
	}
	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_L) && !mlx_is_key_down(ctx->mlx, MLX_KEY_K)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_K) && !mlx_is_key_down(ctx->mlx, MLX_KEY_L)))
	{
		mul = powf(2.0f, 0.5f * ctx->mlx->delta_time);
		div = 1.0f / mul;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_K))
			ctx->scene.cam.focal_len_mm *= div;
		else
			ctx->scene.cam.focal_len_mm *= mul;
		ctx->scene.cam.focal_len_mm = clampf(ctx->scene.cam.focal_len_mm, 14.0f, 800.0f);
		dirty = true;
	}
	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN) && !mlx_is_key_down(ctx->mlx, MLX_KEY_UP)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_UP) && !mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN)))
	{
		mul = powf(2.0f, 0.3f * ctx->mlx->delta_time);
		div = 1.0f / mul;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN))
			ctx->scene.cam.focus_dist *= div;
		else
			ctx->scene.cam.focus_dist *= mul;
		ctx->scene.cam.focus_dist = clampf(ctx->scene.cam.focus_dist, 0.1f, 1000.0f);
		dirty = true;
	}
// 	if (\
// (mlx_is_key_down(ctx->mlx, MLX_KEY_9) && !mlx_is_key_down(ctx->mlx, MLX_KEY_0)) || \
// (mlx_is_key_down(ctx->mlx, MLX_KEY_0) && !mlx_is_key_down(ctx->mlx, MLX_KEY_9)))
// 	{
// 		mul = powf(2.0f, 0.5f * ctx->mlx->delta_time);
// 		div = 1.0f / mul;
// 		if (mlx_is_key_down(ctx->mlx, MLX_KEY_9))
// 			ctx->scene.cam.shutter_speed *= div;
// 		else
// 			ctx->scene.cam.shutter_speed *= mul;
// 		ctx->scene.cam.shutter_speed = clampf(ctx->scene.cam.shutter_speed, 0.0001f, 30.0f);
// 		dirty = true;
// 	}
// 	if (\
// (mlx_is_key_down(ctx->mlx, MLX_KEY_7) && !mlx_is_key_down(ctx->mlx, MLX_KEY_8)) || \
// (mlx_is_key_down(ctx->mlx, MLX_KEY_8) && !mlx_is_key_down(ctx->mlx, MLX_KEY_7)))
// 	{
// 		mul = powf(2.0f, 0.5f * ctx->mlx->delta_time);
// 		div = 1.0f / mul;
// 		if (mlx_is_key_down(ctx->mlx, MLX_KEY_7))
// 			ctx->scene.cam.iso *= div;
// 		else
// 			ctx->scene.cam.iso *= mul;
// 		ctx->scene.cam.iso = clampf(ctx->scene.cam.iso, 100.0f, 51200.0f);
// 		dirty = true;
// 	}
	return (dirty);
}

