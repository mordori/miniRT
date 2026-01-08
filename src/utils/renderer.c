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
	r->width = r->new_width;
	r->height = r->new_height;
	r->pixels = r->width * r->height;
	if (r->buffer)
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
	start_render(r);
}

void	stop_render(t_renderer *r)
{
	pthread_mutex_lock(&r->mutex);
	r->active = false;
	r->resize_pending = false;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
}
