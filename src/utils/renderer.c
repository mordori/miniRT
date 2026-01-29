#include "utils.h"
#include "camera.h"
#include "rendering.h"

static inline uint32_t	vec3_to_uint32(t_vec3 color);

void	blit(const t_context *ctx, const t_renderer *r, uint32_t i)
{
	uint32_t	*pixels;
	t_vec3		*buf;
	t_vec3		color;
	float		scale;
	t_pixel		pixel;

	buf = __builtin_assume_aligned(r->buffer, 64);
	pixels = (uint32_t *)ctx->img->pixels;
	pixel.frame = r->frame;
	if (r->frame > 1)
		scale = 1.0f / (float)r->frame;
	else
	{
		scale = 1.0f;
		pixel.frame = 1;
	}
	pixel.x = 0;
	pixel.y = 0;
	while (i < r->pixels)
	{
		color = vec3_scale(buf[i], scale);
		color = post_process(ctx, &pixel, color);
		pixels[i++] = vec3_to_uint32(color);
		if (++pixel.x == r->width)
		{
			pixel.x = 0;
			++pixel.y;
		}
	}
}

static inline uint32_t	vec3_to_uint32(t_vec3 color)
{
	uint32_t	result;

	result =
		((uint32_t)(0xFFu << 24) |
		((uint32_t)(color.b * 255.0f + 0.5f) << 16) |
		((uint32_t)(color.g * 255.0f + 0.5f) << 8) |
		((uint32_t)(color.r * 255.0f + 0.5f)));
	return (result);
}

void	resize_window(t_context *ctx)
{
	t_renderer	*r;
	size_t		size;

	if (!ctx || !ctx->mlx || !ctx->img)
		return ;
	r = &ctx->renderer;
	pthread_mutex_lock(&r->mutex);
	while (r->active && r->threads_running > 0)
		pthread_cond_wait(&r->cond, &r->mutex);
	r->width = r->new_width;
	r->height = r->new_height;
	r->pixels = r->width * r->height;
	free(r->buffer);
	size = (sizeof(t_vec3) * r->pixels + 63) & ~63;
	r->buffer = aligned_alloc(64, size);
	if (!r->buffer || !mlx_resize_image(ctx->img, r->width, r->height))
	{
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	}
	memset(r->buffer, 0, size);
	update_camera(ctx);
	r->resize_pending = false;
	if (r->frame == 0)
		blit(ctx, &ctx->renderer, 0);
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
	start_render(r, &ctx->scene.cam);
}

void	start_render(t_renderer *r, const t_camera *cam)
{
	pthread_mutex_lock(&r->mutex);
	r->tiles.x = (r->width + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles.y = (r->height + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles_total = r->tiles.x * r->tiles.y;
	r->tile_index = 0;
	r->cam = *cam;
	r->mode = RENDER_REFINE;
	r->ray_bounces = REFINE_BOUNCES;
	r->frame = 1;
	r->render_time = time_now();
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
}

void	stop_render(t_renderer *r)
{
	pthread_mutex_lock(&r->mutex);
	r->active = false;
	r->resize_pending = false;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
}
