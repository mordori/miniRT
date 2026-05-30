#include "camera.h"
#include "rendering.h"
#include "utils.h"

static inline void resize_frame_buffer(t_context* ctx, t_renderer* r);
static inline void resize_selection_mask(t_context* ctx, t_renderer* r);

void resize_hook(int width, int height, void* param) {
	t_context* ctx = (t_context*)param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return;

	t_renderer* r = &ctx->renderer;
	bool is_pending = false;

	pthread_mutex_lock(&r->mutex);
	if (r->active) {
		is_pending = r->resize_pending;
		r->resize_pending = true;
		r->new_width = width;
		r->new_height = height;
		ctx->resize_time = time_now();
		if (!is_pending)
			pthread_cond_broadcast(&r->cond);
	}
	pthread_mutex_unlock(&r->mutex);
}

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
		blit(ctx, &ctx->renderer, false);
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
	free(r->denoise_buffer);

	size_t size = sizeof(t_vec3) * r->pixels;
	r->buffer = a_alloc(64, size);
	r->denoise_buffer = a_alloc(64, size);
	if (!r->buffer || !r->denoise_buffer || !mlx_resize_image(ctx->img, r->width, r->height)) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	}
	memset(r->buffer, 0, size);
	memset(r->denoise_buffer, 0, size);

	if (r->oidn_filter)
		oidnReleaseFilter(r->oidn_filter);
	if (r->oidn_buffer)
		oidnReleaseBuffer(r->oidn_buffer);
	r->oidn_buffer = oidnNewBuffer(r->oidn_device, size);
	r->oidn_filter = oidnNewFilter(r->oidn_device, "RT");
	oidnSetFilterImage(r->oidn_filter, "color", r->oidn_buffer, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter, "output", r->oidn_buffer, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterBool(r->oidn_filter, "hdr", true);
	oidnCommitFilter(r->oidn_filter);
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
