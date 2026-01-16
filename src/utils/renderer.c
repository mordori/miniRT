#include "utils.h"
#include "camera.h"
#include "rendering.h"

void	blit(t_image *img, t_renderer *r)
{
	uint32_t	*pixels;
	t_vec3		color;
	float		scale;
	uint32_t	i;
	uint32_t	limit;
	t_vec3		*buf;

	buf = __builtin_assume_aligned(r->buffer, 64);
	pixels = (uint32_t *)img->pixels;
	limit = r->pixels;
	i = 0;
	scale = 1.0f / (float)r->frame;
	while (i < limit)
	{
		color = vec3_scale(buf[i], scale);
		color = post_process(color);
		pixels[i] = vec3_to_uint32(color);
		++i;
	}
}

uint32_t	vec3_to_uint32(t_vec3 color)
{
	uint32_t	result;

	result =
		((uint32_t)(0xFF << 24) |
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
	r->mode = RENDER_PREVIEW;
	r->ray_bounces = 2;
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
