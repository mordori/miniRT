#include "rendering.h"
#include "utils.h"

static inline void	*render_routine(void *arg);

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
	atomic_store(&r->active, true);
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
	t_vec3		*ptr;
	t_int2		idx;
	t_int2		start;
	t_int2		end;
	uint32_t	tile_id;

	ctx = (t_context *)arg;
	r = &ctx->renderer;
	while (atomic_load(&r->active))
	{
		tile_id = atomic_fetch_add(&r->tile_index, 1);
		if (atomic_load(&r->resize_pending) || tile_id >= r->tiles_total)
		{
			usleep(2000);
			continue ;
		}
		start.x = (tile_id % r->tiles.x) * TILE_SIZE;
		start.y = (tile_id / r->tiles.x) * TILE_SIZE;
		end.x = ft_uint_min(start.x + TILE_SIZE, r->width);
		end.y = ft_uint_min(start.y + TILE_SIZE, r->height);
		idx.y = start.y;
		while (idx.y < end.y)
		{
			ptr = &r->buffer[idx.y * r->width + start.x];
			idx.x = start.x;
			while (idx.x < end.x)
			{
				*ptr = post_process(trace_ray(&ctx->scene, idx.x++, idx.y).rgb);
				++ptr;
			}
			++idx.y;
		}
	}
	return (NULL);
}

bool	start_render(t_renderer *r)
{
	r->tiles.x = (r->width + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles.y = (r->height + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles_total = r->tiles.x * r->tiles.y;
	atomic_store(&r->tile_index, 0);
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
		color =
			((uint32_t)(0xFF << 24) |
			((uint32_t)(r->buffer[i].b * 255.0f + 0.5f) << 16) |
			((uint32_t)(r->buffer[i].g * 255.0f + 0.5f) << 8) |
			((uint32_t)(r->buffer[i].r * 255.0f + 0.5f)));
		pixels[i] = color;
		++i;
	}
}
