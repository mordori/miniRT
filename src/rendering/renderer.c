#include "rendering.h"
#include "utils.h"

static inline void	*render_routine(void *arg);
static inline void	render_pixel(t_context *ctx, const uint32_t x, const uint32_t y);

bool	init_renderer(t_context *ctx)
{
	t_renderer	*r;

	r = &ctx->renderer;
	r->threads_amount = sysconf(_SC_NPROCESSORS_ONLN);
	if (r->threads_amount == ERROR)
		r->threads_amount = THREADS_DFL;
	r->threads = malloc(sizeof(*r->threads) * r->threads_amount);
	if (!r->threads)
		fatal_error(ctx, errors(ERR_RENINIT), __FILE__, __LINE__);
	r->active = true;
	r->paused = false;
	while (r->threads_init < r->threads_amount)
	{
		if (pthread_create(&r->threads[r->threads_init], NULL, render_routine, ctx))
			break ;
		++r->threads_init;
	}
	return (r->threads_init == r->threads_amount);
}

static inline void	*render_routine(void *arg)
{
	t_context	*ctx;
	t_renderer	*r;
	uint32_t	i;

	ctx = (t_context *)arg;
	r = &ctx->renderer;
	while (r->active)
	{
		i = atomic_fetch_add(&r->pixel_index, 1);
		if (i >= r->pixels)
		{
			usleep(2000);
			continue ;
		}
		render_pixel(ctx, i % r->width, i / r->width);
		if (atomic_fetch_add(&r->pixels_done, 1) == r->pixels - 1)
			r->finished = true;
	}
	return (NULL);
}

static inline void	render_pixel(t_context *ctx, const uint32_t x, const uint32_t y)
{
	t_vec4		color;
	uint32_t	i;

	color = trace_ray(&ctx->scene, x, y);
	i = (y * ctx->img->width + x);
	ctx->renderer.buffer[i] = color.rgb;
}

bool	start_render(t_renderer *r)
{
	atomic_store(&r->pixel_index, 0);
	atomic_store(&r->pixels_done, 0);
	r->finished = false;
	return (true);
}

void	blit(t_image *img, t_renderer *r)
{
	uint32_t	*pixels;
	uint32_t	color;
	uint32_t	i;

	pixels = (uint32_t *)img->pixels;
	i = 0;
	while (i < r->pixels)
	{
		color = \
((0xFF << 24) | \
((uint32_t)(ft_clamp01(r->buffer[i].b) * 255.0f + 0.5f) << 16) | \
((uint32_t)(ft_clamp01(r->buffer[i].g) * 255.0f + 0.5f) << 8) | \
((uint32_t)(ft_clamp01(r->buffer[i].r) * 255.0f + 0.5f)));
		pixels[i] = color;
		++i;
	}
}
