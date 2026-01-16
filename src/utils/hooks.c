#include "utils.h"
#include "input.h"
#include "rendering.h"
#include "camera.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = (t_context *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
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
	bool		is_pending;

	ctx = (t_context *)param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	r = &ctx->renderer;
	is_pending = false;
	pthread_mutex_lock(&r->mutex);
	if (r->active)
	{
		is_pending = r->resize_pending;
		r->resize_pending = true;
		r->new_width = width;
		r->new_height = height;
		ctx->resize_time = time_now();
		if (!is_pending)
			pthread_cond_broadcast(&r->cond);
	}
	pthread_mutex_unlock(&r->mutex);
}

void	loop_hook(void *param)
{
	t_context		*ctx;
	t_renderer		*r;
	bool			resize;
	static bool		update;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	if (process_input(ctx))
		update = true;
	pthread_mutex_lock(&r->mutex);
	resize = r->resize_pending;
	if (r->frame_complete)
	{
		r->frame_complete = false;
		blit(ctx->img, r);
		++r->frame;
	}
	if (r->threads_running == 0 && !resize)
	{
		if (update)
		{
			r->cam = ctx->scene.cam;
			r->mode = RENDER_PREVIEW;
			r->ray_bounces = PREVIEW_BOUNCES;
			r->frame = 1;
			r->tile_index = 0;
			update = false;
			pthread_cond_broadcast(&r->cond);
		}
		else if (r->frame < RENDER_SAMPLES)
		{
			if (r->mode == RENDER_PREVIEW)
				r->frame = 1;
			r->mode = RENDER_REFINE;
			r->ray_bounces = REFINE_BOUNCES;
			r->tile_index = 0;
			pthread_cond_broadcast(&r->cond);
		}
	}
	pthread_mutex_unlock(&r->mutex);
	if (resize && ctx->resize_time != 0 && resize_timer(ctx))
		resize_window(ctx);
}
