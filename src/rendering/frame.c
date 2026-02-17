#include "rendering.h"
#include "input.h"
#include "utils.h"

static inline void	process_frame(t_context *ctx, t_renderer *r);
static inline void	copy_frame_buffer(const t_context *ctx, t_vec3 *buf, uint32_t *pixels, t_pixel *pixel);
static inline void	copy_frame_buffer_preview(const t_context *ctx, t_vec3 *buf, uint32_t *pixels);

void	frame_loop(void *param)
{
	t_context		*ctx;
	t_renderer		*r;
	static bool		update;

	ctx = (t_context *)param;
	r = &ctx->renderer;
	process_input(ctx, &update);
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
	else if (!r->threads_running && r->frame < r->render_samples + 1)
		set_render_refine(r);
	pthread_mutex_unlock(&r->mutex);
	limit_polling_rate(r);
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
	if (r->mode == RENDER_REFINE)
		copy_frame_buffer(ctx, buf, pixels, &pixel);
	else
		copy_frame_buffer_preview(ctx, buf, pixels);
}

static inline void	copy_frame_buffer(const t_context *ctx, t_vec3 *buf, uint32_t *pixels, t_pixel *pixel)
{
	t_vec3				color;
	uint32_t			i;
	const uint32_t		limit = ctx->renderer.pixels;
	const uint32_t		width = ctx->renderer.width;

	i = 0;
#pragma unroll 4
	while (i < limit)
	{
		color = vec3_scale(buf[i], pixel->scale);
		color = post_process(ctx, pixel, color);
		pixels[i++] = vec3_to_uint32(color);
		if (pixel->x++ == width)
		{
			pixel->x = 0;
			pixel->y++;
		}
	}
}

static inline void	copy_frame_buffer_preview(const t_context *ctx, t_vec3 *buf, uint32_t *pixels)
{
	t_vec3				color;
	uint32_t			i;
	const uint32_t		limit = ctx->renderer.pixels;

	i = 0;
	while (i < limit)
	{
		color = buf[i];
		color = post_process_preview(ctx, color);
		pixels[i++] = vec3_to_uint32(color);
	}
}

static inline void	process_frame(t_context *ctx, t_renderer *r)
{
	uint32_t		render_time;

	print_render_status(ctx, r);
	if (r->mode != RENDER_REFINE || r->frame < 8 || (r->frame < 16 && (r->frame & 1)) || r->frame == 32 || r->frame == 48 || r->frame == 64 || r->frame == 80 || (time_now() > r->blit_time + 5000 || r->frame == r->render_samples))
	{
		blit(ctx, r);
		r->blit_time = time_now();
		render_time = time_now() - r->render_time;
		if (r->frame == r->render_samples)
			printf("\nDone!\t\t    Time: %.1fs\n\n", render_time / 1000.0f);
	}
	r->frame_complete = false;
	++r->frame;
}
