#include "utils.h"
#include "camera.h"
#include "rendering.h"

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
	free(r->buffer_a);
	free(r->buffer_b);
	size = (sizeof(t_vec3) * r->pixels + 63) & ~63;
	r->buffer_a = aligned_alloc(64, size);
	r->buffer_b = aligned_alloc(64, size);
	if (!r->buffer_a || !r->buffer_b || !mlx_resize_image(ctx->img, r->width, r->height))
	{
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	}
	memset(r->buffer_a, 0, size);
	memset(r->buffer_b, 0, size);
	update_camera(ctx);
	r->resize_pending = false;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
	start_render(r, &ctx->scene.cam);
}

void	stop_render(t_renderer *r)
{
	pthread_mutex_lock(&r->mutex);
	r->active = false;
	r->resize_pending = false;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
}
