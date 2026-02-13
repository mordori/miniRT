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

void	print_render_status(t_context *ctx, t_renderer *r)
{
	int					step;
	int					i;
	static const char	empty[INT64_LENGTH] = {0};
	char				str[INT64_LENGTH];

	memcpy(str, empty, INT64_LENGTH);
	try_write(ctx, STDOUT_FILENO, "\rRendering...   ");
	try_write(ctx, STDOUT_FILENO, "[");
	step = r->frame / ((RENDER_SAMPLES) / 19);
	i = step;
	while (i-- > 0)
		try_write(ctx, STDOUT_FILENO, "#");
	i = 19 - step;
	while (i-- > 0)
		try_write(ctx, STDOUT_FILENO, " ");
	try_write(ctx, STDOUT_FILENO, "] [");
	uint64_to_str(r->frame, str);
	try_write(ctx, STDOUT_FILENO, str);
	try_write(ctx, STDOUT_FILENO, "/");
	uint64_to_str(RENDER_SAMPLES, str);
	try_write(ctx, STDOUT_FILENO, str);
	try_write(ctx, STDOUT_FILENO, "] ");
}

void	set_render_preview(t_context *ctx, t_renderer *r, bool *update)
{
	r->cam = ctx->scene.cam;
	if (r->mode == RENDER_REFINE)
		r->mode = RENDER_PREVIEW;
	r->ray_bounces = PREVIEW_BOUNCES;
	r->frame = 1;
	r->tile_index = 0;
	*update = false;
	r->render_time = time_now();
	pthread_cond_broadcast(&r->cond);
}

void	set_render_refine(t_renderer *r)
{
	if (r->mode == RENDER_PREVIEW)
	{
		r->blit_time = 0;
		r->render_time = time_now();
		r->frame = 1;
	}
	r->mode = RENDER_REFINE;
	r->ray_bounces = REFINE_BOUNCES;
	r->tile_index = 0;
	pthread_cond_broadcast(&r->cond);
}

void	cancel_render(t_renderer *r)
{
	r->tile_index = r->tiles_total;
	pthread_cond_broadcast(&r->cond);
	while (r->threads_running)
		pthread_cond_wait(&r->cond, &r->mutex);
	atomic_store(&r->render_cancel, false);
	r->frame_complete = false;
}

void	process_frame(t_context *ctx, t_renderer *r)
{
	uint32_t		render_time;

	print_render_status(ctx, r);
	if (r->mode != RENDER_REFINE || r->frame < 8 || (r->frame < 16 && (r->frame & 1)) || r->frame == 32 || r->frame == 48 || r->frame == 64 || r->frame == 80 || (time_now() > r->blit_time + 5000 || r->frame == RENDER_SAMPLES))
	{
		blit(ctx, r, 0, r->pixels);
		r->blit_time = time_now();
		render_time = time_now() - r->render_time;
		if (r->frame == RENDER_SAMPLES)
			printf("\nDone!\t\t    Time: %.1fs\n\n", render_time / 1000.0f);
	}
	r->frame_complete = false;
	++r->frame;
}

void	loop_hook(void *param)
{
	t_context		*ctx;
	t_renderer		*r;
	static bool		update;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	process_input(ctx, r, &update);
	pthread_mutex_lock(&r->mutex);
	if (atomic_load(&r->render_cancel) || r->resize_pending)
		cancel_render(r);
	else if (r->frame_complete && !r->resize_pending)
		process_frame(ctx, r);
	if (r->resize_pending)
	{
		pthread_mutex_unlock(&r->mutex);
		if (ctx->resize_time != 0 && resize_timer(ctx))
			resize_window(ctx);
		return ;
	}
	else if (!r->threads_running && update)
		set_render_preview(ctx, r, &update);
	else if (!r->threads_running && r->frame < RENDER_SAMPLES + 1)
		set_render_refine(r);
	pthread_mutex_unlock(&r->mutex);
}
