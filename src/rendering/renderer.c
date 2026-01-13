#include "rendering.h"
#include "utils.h"
#include "libft_random.h"

static inline void	*render_routine(void *arg);
static inline void	render_tile(const t_renderer *r, t_vec3 *buf, uint32_t tile_id, const t_scene *scene);
static inline void render_pixel(const t_renderer *r, const t_scene *scene, t_int2 idx, t_vec3 *pixel);

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
		render_tile(r, r->buffer, tile_id, &ctx->scene);
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

static inline void	render_tile(const t_renderer *r, t_vec3 *buf, uint32_t tile_id, const t_scene *scene)
{
	t_vec3			*pixel;
	t_int2			idx;
	t_int2			start;
	t_int2			end;
	uint32_t		width;

	start.x = (tile_id % r->tiles.x) * TILE_SIZE;
	start.y = (tile_id / r->tiles.x) * TILE_SIZE;
	end.x = ft_uint_min(start.x + TILE_SIZE, r->width);
	end.y = ft_uint_min(start.y + TILE_SIZE, r->height);
	idx.y = start.y;
	width = r->width;
	buf = __builtin_assume_aligned(buf, 64);
	while (idx.y < end.y)
	{
		pixel = &buf[idx.y * width + start.x];
		idx.x = start.x;
		while (idx.x < end.x)
		{
			render_pixel(r, scene, idx, pixel);
			++pixel;
			++idx.x;
		}
		++idx.y;
	}
}

static inline void render_pixel(const t_renderer *r, const t_scene *scene, t_int2 idx, t_vec3 *pixel)
{
	uint32_t		seed;
	t_vec2			uv;
	t_vec3			color;

	seed = hash_lowerbias32((idx.x * r->width + idx.y) ^ hash_lowerbias32(r->frame));
	if (seed == 0)
		seed = 1;
	if (r->mode == RENDER_PREVIEW)
	{
		uv.u = (float)idx.x + 0.5f;
		uv.v = (float)idx.y + 0.5f;
	}
	else
	{
		uv.u = (float)idx.x + randomf01(&seed);
		uv.v = (float)idx.y + randomf01(&seed);
	}
	color = trace_path(scene, r, uv, &seed);
	if (r->mode == RENDER_PREVIEW || r->frame == 1)
		*pixel = color;
	else
		*pixel = vec3_add(*pixel, color);
}
