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
	t_renderer	*r;

	ctx = param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	r = &ctx->renderer;
	if (r->buffer)
		free(r->buffer);
	r->buffer = malloc(sizeof(t_vec3) * (width * height));
	if (!r->buffer || !mlx_resize_image(ctx->img, width, height))
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	*r->buffer = (t_vec3){0};
	r->width = width;
	r->height = height;
	r->pixels = width * height;
	update_camera(ctx);
	if (r->threads_init == r->threads_amount)
		start_render(r);
}

void	update_hook(void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	if (process_input(ctx))
		start_render(&ctx->renderer);
	if (ctx->renderer.finished)
		post_process(ctx->renderer.buffer);
	blit(ctx->img, &ctx->renderer);
}
