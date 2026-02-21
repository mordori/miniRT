#include "input.h"

bool	is_key_down(t_context *ctx)
{
	bool		dirty;

	dirty = false;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_PERIOD) && !mlx_is_key_down(ctx->mlx, MLX_KEY_COMMA))
	{
		ctx->scene.cam.fov += ctx->mlx->delta_time;
		dirty = true;
	}
	if (!mlx_is_key_down(ctx->mlx, MLX_KEY_PERIOD) && mlx_is_key_down(ctx->mlx, MLX_KEY_COMMA))
	{
		ctx->scene.cam.fov -= ctx->mlx->delta_time;
		dirty = true;
	}
	if (dirty)
		ctx->scene.cam.fov = clampf(ctx->scene.cam.fov, M_EPSILON, M_PI - M_EPSILON);
	return (dirty);
}
