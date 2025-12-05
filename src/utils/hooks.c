#include "input.h"
#include "rendering.h"
#include "utils.h"
#include "camera.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
}

void	resize_hook(int width, int height, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	if (!mlx_resize_image(ctx->img, width, height))
	{
		fatal_error(ctx, "resize failed");
	}
	// update_viewport();
	// frame_scene();
}

void	update_hook(void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	// process_input(ctx);
	update_cam(ctx);
}
