#include "utils.h"
#include "input.h"
#include "rendering.h"
#include "camera.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;
	t_renderer	*r;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
	{
		pthread_mutex_lock(&r->mutex);
		r->active = false;
		pthread_cond_broadcast(&r->cond);
		pthread_mutex_unlock(&r->mutex);
		mlx_close_window(ctx->mlx);
	}
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void* param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
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
	pthread_mutex_lock(&r->mutex);
	r->resize_pending = true;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
	ctx->resize_time = time_now();
	r->width = width;
	r->height = height;
	r->pixels = width * height;
}

void	loop_hook(void *param)
{
	t_context	*ctx;
	t_renderer	*r;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	if (!atomic_load(&r->resize_pending))
	{
		if (process_input(ctx))
			start_render(r);
		blit(ctx->img, r);
	}
	else if (ctx->resize_time != 0 && resize_timer(ctx))
		resize_window(ctx);
}
