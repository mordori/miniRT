#include "utils.h"

void	start_render(t_renderer *r, const t_camera *cam)
{
	pthread_mutex_lock(&r->mutex);
	r->tiles.x = (r->width + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles.y = (r->height + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles_total = r->tiles.x * r->tiles.y;
	r->tile_index = 0;
	r->cam = *cam;
	r->mode = RENDER_REFINE;
	r->ray_bounces = r->refine_bounces;
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

void	cancel_render(t_renderer *r)
{
	r->tile_index = r->tiles_total;
	pthread_cond_broadcast(&r->cond);
	while (r->threads_running)
		pthread_cond_wait(&r->cond, &r->mutex);
	atomic_store(&r->render_cancel, false);
	r->frame_complete = false;
}

void	set_render_preview(t_context *ctx, t_renderer *r, bool *update)
{
	r->cam = ctx->scene.cam;
	if (r->mode == RENDER_REFINE)
		r->mode = RENDER_PREVIEW;
	r->ray_bounces = PREVIEW_BOUNCES;
	r->frame = 1;
	r->tile_index = 0;
	*update = false;
	r->render_time = time_now();
	pthread_cond_broadcast(&r->cond);
}

void	set_render_refine(t_renderer *r)
{
	if (r->mode == RENDER_PREVIEW)
	{
		r->blit_time = 0;
		r->render_time = time_now();
		r->frame = 1;
	}
	r->mode = RENDER_REFINE;
	r->ray_bounces = r->refine_bounces;
	r->tile_index = 0;
	pthread_cond_broadcast(&r->cond);
}
