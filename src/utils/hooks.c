#include "utils.h"
#include "input.h"
#include "rendering.h"
#include "camera.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
	{
		atomic_store(&ctx->renderer.active, false);
		mlx_close_window(ctx->mlx);
	}
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void* param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	if (atomic_load(&ctx->renderer.resize_pending) &&
		button == MLX_MOUSE_BUTTON_LEFT &&
		action == MLX_RELEASE)
		resize_window(ctx);
	(void)ctx;
	(void)button;
	(void)action;
	(void)mods;
}

void	cursor_hook(double xpos, double ypos, void* param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	(void)ctx;
	(void)xpos;
	(void)ypos;
}

void	resize_hook(int width, int height, void *param)
{
	t_context	*ctx;
	t_renderer	*r;

	ctx = (t_context *)param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	r = &ctx->renderer;
	atomic_store(&r->resize_pending, true);
	r->width = width;
	r->height = height;
	r->pixels = width * height;
}

void	loop_hook(void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	if (process_input(ctx))
		start_render(&ctx->renderer);
	blit(ctx->img, &ctx->renderer);
}
