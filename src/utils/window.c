#include "camera.h"
#include "rendering.h"
#include "utils.h"

static inline void resize_frame_buffer(t_context* ctx, t_renderer* r);
static inline void resize_selection_mask(t_context* ctx, t_renderer* r);

void resize_window(t_context* ctx) {
	if (!ctx || !ctx->mlx || !ctx->img)
		return;

	t_renderer* r = &ctx->renderer;

	pthread_mutex_lock(&r->mutex);
	while (r->active && r->threads_running > 0)
		pthread_cond_wait(&r->cond, &r->mutex);

	resize_frame_buffer(ctx, r);
	resize_selection_mask(ctx, r);
	update_camera(ctx, &ctx->scene.cam);
	r->resize_pending = false;
	if (r->frame == 0) {
		reset_camera(ctx);
		blit(ctx, &ctx->renderer);
	}
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);

	start_render(r, &ctx->scene.cam);
}

static inline void resize_frame_buffer(t_context* ctx, t_renderer* r) {
	r->width = r->new_width;
	r->height = r->new_height;
	r->pixels = r->width * r->height;
	free(r->buffer);

	size_t size = sizeof(t_vec3) * r->pixels;
	r->buffer = a_alloc(64, size);
	if (!r->buffer || !mlx_resize_image(ctx->img, r->width, r->height)) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	}
	memset(r->buffer, 0, size);
}

static inline void resize_selection_mask(t_context* ctx, t_renderer* r) {
	free(ctx->editor.selection_mask);

	size_t size = sizeof(float) * r->pixels;
	ctx->editor.selection_mask = malloc(size);
	if (!ctx->editor.selection_mask) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	}
	memset(ctx->editor.selection_mask, 0, size);
}
