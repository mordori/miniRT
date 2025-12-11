#include "utils.h"
#include "input.h"
#include "rendering.h"
#include "camera.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
	{
		ctx->renderer.active = false;
		mlx_close_window(ctx->mlx);
	}
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void* param)
{
	t_context	*ctx;

	ctx = param;
	(void)ctx;
	(void)button;
	(void)action;
	(void)mods;
}

void	cursor_hook(double xpos, double ypos, void* param)
{
	t_context	*ctx;

	ctx = param;
	(void)ctx;
	(void)xpos;
	(void)ypos;
}

void	resize_hook(int width, int height, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	if (ctx->renderer.buffer)
		free(ctx->renderer.buffer);
	ctx->renderer.buffer = malloc(sizeof(t_vec3) * (width * height));
	if (!ctx->renderer.buffer || !mlx_resize_image(ctx->img, width, height))
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	*ctx->renderer.buffer = (t_vec3){0};
	ctx->renderer.width = width;
	ctx->renderer.height = height;
	ctx->renderer.pixels = width * height;
	// update_cam();
	// update_viewport();
}

void	update_hook(void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	process_input(ctx);
}
