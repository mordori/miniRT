/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:52 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/30 15:08:56 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "input.h"
#include "utils.h"
#include "ui.h"

static inline void	process_frame(t_context *ctx, t_renderer *r);
static inline void	\
set_renderer_state(t_context *ctx, t_renderer *r, bool *update);
static inline bool	is_active(t_context *ctx);

void	frame_loop(void *param)
{
	t_context		*ctx;
	t_renderer		*r;
	static bool		update;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	pthread_mutex_lock(&r->mutex);
	process_input(ctx, &update);
	if (atomic_load(&r->render_cancel) || r->resize_pending)
	{
		cancel_render(r);
		update = true;
	}
	else if (r->frame_complete && !r->resize_pending)
		process_frame(ctx, r);
	set_renderer_state(ctx, r, &update);
	pthread_mutex_unlock(&r->mutex);
	update_ui(ctx);
	limit_polling_rate(r);
}

static inline void	set_renderer_state(\
t_context *ctx, t_renderer *r, bool *update)
{
	if (r->resize_pending)
	{
		pthread_mutex_unlock(&r->mutex);
		if (ctx->resize_time != 0 && resize_timer(ctx))
			resize_window(ctx);
		return ;
	}
	if (r->threads_running)
		return ;
	if (is_active(ctx) || (r->mode == SOLID && *update))
		set_mode_preview(ctx, r, update);
	else if (*update)
	{
		*update = false;
		set_mode_rendered(r);
	}
	else if (r->mode != SOLID && r->frame < r->render_samples + 1)
		set_mode_rendered(r);
}

static inline bool	is_active(t_context *ctx)
{
	mlx_t		*m;

	if (ctx->scene.cam.state != CAM_DEFAULT || ctx->editor.mode != EDIT_DEFAULT)
		return (true);
	if (ctx->renderer.mode == SOLID)
		return (false);
	m = ctx->mlx;
	if (mlx_is_key_down(m, KEY_FORWARD) || mlx_is_key_down(m, KEY_BACK) || \
mlx_is_key_down(m, KEY_RIGHT) || mlx_is_key_down(m, KEY_LEFT) || \
mlx_is_key_down(m, KEY_UP) || mlx_is_key_down(m, KEY_DOWN))
		return (true);
	if (mlx_is_key_down(m, MLX_KEY_L) || mlx_is_key_down(m, MLX_KEY_K) || \
mlx_is_key_down(m, MLX_KEY_COMMA) || mlx_is_key_down(m, MLX_KEY_PERIOD))
		return (true);
	if (mlx_is_key_down(m, MLX_KEY_LEFT) || mlx_is_key_down(m, MLX_KEY_RIGHT) || \
mlx_is_key_down(m, MLX_KEY_UP) || mlx_is_key_down(m, MLX_KEY_DOWN))
		return (true);
	return (false);
}

void	blit(const t_context *ctx, const t_renderer *r)
{
	t_vec3			*buf;
	uint32_t		*pixels;
	t_pixel			pixel;

	pixel = (t_pixel){0};
	pixels = (uint32_t *)((void *)ctx->img->pixels);
	buf = __builtin_assume_aligned(r->buffer, 64);
	pixel.frame = r->frame;
	if (pixel.frame > 1)
		pixel.scale = 1.0f / (float)pixel.frame;
	else
	{
		pixel.scale = 1.0f;
		pixel.frame = 1;
	}
	if (r->mode == RENDERED)
		copy_frame_buffer(ctx, buf, pixels, &pixel);
	else
		copy_frame_buffer_preview(ctx, r->width, buf, pixels);
}

static inline void	process_frame(t_context *ctx, t_renderer *r)
{
	uint32_t		render_time;
	static char		buf[128];

	print_render_status(ctx, r);
	if (\
r->mode != RENDERED || r->frame < 8 || (r->frame < 16 && (r->frame & 1)) || \
r->frame == 32 || r->frame == 48 || r->frame == 64 || r->frame == 80 || \
(time_now() > r->blit_time + 5000 || r->frame >= r->render_samples))
	{
		blit(ctx, r);
		r->blit_time = time_now();
		render_time = time_now() - r->render_time;
		if (r->frame >= r->render_samples)
		{
			snprintf(buf, sizeof(buf), \
"\r\033[K\033[1;32mDone!   Frames: %d   Render time: %.1fs\033[0m", \
r->frame, render_time / 1000.0f);
			try_write(ctx, STDOUT_FILENO, buf);
		}
	}
	r->frame_complete = false;
	++r->frame;
}
