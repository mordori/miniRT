#include "utils.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;
	bool		dirty;

	ctx = (t_context *)param;
	dirty = false;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_RELEASE)
	{
		dirty = true;
		if (ctx->renderer.mode != SOLID)
			ctx->renderer.mode = SOLID;
		else
			ctx->renderer.mode = RENDERED;
	}
	if (keydata.key == MLX_KEY_O && keydata.action == MLX_RELEASE)
	{
		dirty = true;
		ctx->renderer.render_samples >>= 1u;
		if (ctx->renderer.render_samples < 2u)
			ctx->renderer.render_samples = 2u;
	}
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_RELEASE)
	{
		dirty = true;
		ctx->renderer.render_samples <<= 1u;
		if (ctx->renderer.render_samples > 8192u)
			ctx->renderer.render_samples = 8192u;
	}
	if (keydata.key == MLX_KEY_U && keydata.action == MLX_RELEASE)
	{
		dirty = true;
		ctx->renderer.render_bounces >>= 1u;
		if (ctx->renderer.render_bounces < 2u)
			ctx->renderer.render_bounces = 2u;
	}
	if (keydata.key == MLX_KEY_I && keydata.action == MLX_RELEASE)
	{
		dirty = true;
		ctx->renderer.render_bounces <<= 1u;
		if (ctx->renderer.render_bounces > 128u)
			ctx->renderer.render_bounces = 128u;
	}
	if (keydata.key == MLX_KEY_N && keydata.action == MLX_RELEASE)
		ctx->hide_stats = !ctx->hide_stats;
	if (dirty)
		atomic_store(&ctx->renderer.render_cancel, true);
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
