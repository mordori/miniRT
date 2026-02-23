#include "input.h"

bool	is_key_down(t_context *ctx)
{
	bool		dirty;
	float		d_focal_len;
	float		d_f_stop_d;
	float		d_focus_dist;

	dirty = false;
	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT) && !mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_RIGHT) && !mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT)))
	{
		d_f_stop_d = ctx->scene.cam.f_stop * ctx->mlx->delta_time * 0.35f;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT))
			d_f_stop_d = -d_f_stop_d;
		ctx->scene.cam.f_stop = clampf(ctx->scene.cam.f_stop + d_f_stop_d, 0.1f, 128.0f);
		dirty = true;
	}
	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_L) && !mlx_is_key_down(ctx->mlx, MLX_KEY_K)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_K) && !mlx_is_key_down(ctx->mlx, MLX_KEY_L)))
	{
		d_focal_len = ctx->scene.cam.focal_len_mm * ctx->mlx->delta_time * 0.35f;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_K))
			d_focal_len = -d_focal_len;
		ctx->scene.cam.focal_len_mm = clampf(ctx->scene.cam.focal_len_mm + d_focal_len, 1.0f, 1000.0f);
		dirty = true;
	}
	if (\
(mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN) && !mlx_is_key_down(ctx->mlx, MLX_KEY_UP)) || \
(mlx_is_key_down(ctx->mlx, MLX_KEY_UP) && !mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN)))
	{
		d_focus_dist = ctx->scene.cam.focus_dist * ctx->mlx->delta_time * 0.35f;
		if (mlx_is_key_down(ctx->mlx, MLX_KEY_DOWN))
			d_focus_dist = -d_focus_dist;
		ctx->scene.cam.focus_dist = clampf(ctx->scene.cam.focus_dist + d_focus_dist, 0.1f, M_INF);
		dirty = true;
	}
	return (dirty);
}
