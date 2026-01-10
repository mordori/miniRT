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
	t_vec3			*temp;
	bool			draw_frame;
	static bool		update;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	if (process_input(ctx))
		update = true;
	pthread_mutex_lock(&r->mutex);
	resize = r->resize_pending;
	draw_frame = false;
	if (r->frame_complete)
	{
		temp = r->buffer_b;
		r->buffer_b = r->buffer_a;
		r->buffer_a = temp;
		r->frame_complete = false;
		draw_frame = true;
	}
	if (update && !resize && r->threads_running == 0)
	{
		r->cam = ctx->scene.cam;
		r->tile_index = 0;
		pthread_cond_broadcast(&r->cond);
		update = false;
	}
	pthread_mutex_unlock(&r->mutex);
	if (resize && ctx->resize_time != 0 && resize_timer(ctx))
		resize_window(ctx);
	else if (draw_frame)
		blit(ctx->img, r);
}
