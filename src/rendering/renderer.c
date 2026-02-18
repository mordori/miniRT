#include "rendering.h"
#include "utils.h"

static inline void	*render_routine(void *arg);
static inline void	render_tile(const t_context *ctx, t_vec3 *buf, uint32_t tile_id);
static inline void	render_pixel(const t_context *ctx, t_pixel *pixel);

bool	init_renderer(t_context *ctx)
{
	t_renderer		*r;

	r = &ctx->renderer;
	r->init_mutex = !pthread_mutex_init(&r->mutex, NULL);
	r->init_cond = !pthread_cond_init(&r->cond, NULL);
	r->threads_amount = (int32_t)sysconf(_SC_NPROCESSORS_ONLN);
	if (r->threads_amount == ERROR)
		r->threads_amount = THREADS_DFL;
	r->threads_amount *= 2;
	r->threads = malloc(sizeof(*r->threads) * r->threads_amount);
	if (!r->init_mutex || !r->init_cond || !r->threads)
		fatal_error(ctx, errors(ERR_RENINIT), __FILE__, __LINE__);
	r->active = true;
	while (r->threads_init < r->threads_amount)
	{
		if (pthread_create(&r->threads[r->threads_init], NULL, render_routine, ctx))
		{
			pthread_mutex_lock(&r->mutex);
			r->active = false;
			pthread_mutex_unlock(&r->mutex);
			break ;
		}
		++r->threads_init;
	}
	return (r->threads_init == r->threads_amount);
}

static inline void	*render_routine(void *arg)
{
	t_context		*ctx;
	t_renderer		*r;
	uint32_t		tile_id;

	ctx = (t_context *)arg;
	r = &ctx->renderer;
	pthread_mutex_lock(&r->mutex);
	while (true)
	{
		while (r->active && (r->resize_pending || r->tile_index >= r->tiles_total))
			pthread_cond_wait(&r->cond, &r->mutex);
		if (!r->active)
		{
			pthread_mutex_unlock(&r->mutex);
			break ;
		}
		tile_id = r->tile_index++;
		++r->threads_running;
		pthread_mutex_unlock(&r->mutex);
		render_tile(ctx, r->buffer, tile_id);
		pthread_mutex_lock(&r->mutex);
		--r->threads_running;
		if (r->threads_running == 0 && (r->resize_pending || r->tile_index >= r->tiles_total))
		{
			r->frame_complete = true;
			pthread_cond_broadcast(&r->cond);
		}
	}
	return (NULL);
}

static inline void	render_tile(const t_context *ctx, t_vec3 *buf, uint32_t tile_id)
{
	t_pixel			pixel;
	t_uint2			start;
	t_uint2			end;
	uint32_t		width;

	start.x = (tile_id % ctx->renderer.tiles.x) * TILE_SIZE;
	start.y = (tile_id / ctx->renderer.tiles.x) * TILE_SIZE;
	end.x = ft_uint_min(start.x + TILE_SIZE, ctx->renderer.width);
	end.y = ft_uint_min(start.y + TILE_SIZE, ctx->renderer.height);
	pixel.y = start.y;
	width = ctx->renderer.width;
	buf = __builtin_assume_aligned(buf, 64);
	while (pixel.y < end.y && !atomic_load(&ctx->renderer.render_cancel))
	{
		pixel.color = &buf[pixel.y * width + start.x];
		pixel.x = start.x;
		while (pixel.x < end.x)
		{
			render_pixel(ctx, &pixel);
			++pixel.color;
			++pixel.x;
		}
		++pixel.y;
	}
}

static inline void render_pixel(const t_context *ctx, t_pixel *pixel)
{
	const t_renderer	*r;
	uint32_t			seed;
	t_vec3				color;

	r = &ctx->renderer;
	seed = hash_lowerbias32((pixel->y * r->width + pixel->x) ^ hash_lowerbias32(r->frame));
	if (seed == 0)
		seed = 1;
	pixel->seed = &seed;
	pixel->frame = r->frame;
	if (r->mode == RENDERED)
	{
		pixel->u = (float)pixel->x + blue_noise(&ctx->tex_bn, pixel, BN_PX_U);
		pixel->v = (float)pixel->y + blue_noise(&ctx->tex_bn, pixel, BN_PX_V);
	}
	else
	{
		pixel->u = (float)pixel->x + 0.5f;
		pixel->v = (float)pixel->y + 0.5f;
	}
	color = trace_path(ctx, pixel, r->mode, r->ray_bounces);
	if (r->frame == 1)
		*pixel->color = color;
	else
		*pixel->color = vec3_add(*pixel->color, color);
}
