#include <stdbool.h>
#include <stdint.h>

#include "camera.h"
#include "defines.h"
#include "rendering.h"
#include "utils.h"

static inline void resize_frame_buffer(t_context* ctx, t_renderer* r);
static inline void resize_selection_mask(t_context* ctx, t_renderer* r);

void resize_hook(int width, int height, void* param) {
	static uint32_t i;
	t_context* ctx = (t_context*)param;
	if (!ctx || !ctx->mlx || !ctx->img || width <= UI_WIDTH || height <= UI_BOTTOM)
		return;
	if (ctx->renderer.width + UI_WIDTH == (uint32_t)width && ctx->renderer.height + UI_BOTTOM == (uint32_t)height)
		return;

	t_renderer* r = &ctx->renderer;
	bool is_pending = false;

	pthread_mutex_lock(&r->mutex);
	if (r->active) {
		is_pending = r->resize_pending;
		r->resize_pending = true;
		r->new_width = width;
		r->new_height = height;
		if (i != 0) {
			r->new_width -= UI_WIDTH;
			r->new_height -= UI_BOTTOM;
		}
		ctx->resize_time = time_now();
		if (!is_pending)
			pthread_cond_broadcast(&r->cond);
	}
	pthread_mutex_unlock(&r->mutex);
	++i;
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
	free(r->albedo_buffer);
	free(r->normal_buffer);

	size_t size = sizeof(t_vec3) * r->pixels;
	r->buffer = a_alloc(64, size);
	r->denoise_buffer = a_alloc(64, size);
	r->albedo_buffer = a_alloc(64, size);
	r->normal_buffer = a_alloc(64, size);
	if (!r->buffer || !r->denoise_buffer || !r->albedo_buffer || !r->normal_buffer || !mlx_resize_image(ctx->img, r->width, r->height)) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_RESIZE), __FILE__, __LINE__);
	}
	memset(r->buffer, 0, size);
	memset(r->denoise_buffer, 0, size);
	memset(r->albedo_buffer, 0, size);
	memset(r->normal_buffer, 0, size);

	if (r->oidn_buffer)
		oidnReleaseBuffer(r->oidn_buffer);
	if (r->oidn_albedo)
		oidnReleaseBuffer(r->oidn_albedo);
	if (r->oidn_normal)
		oidnReleaseBuffer(r->oidn_normal);
	if (r->oidn_filter)
		oidnReleaseFilter(r->oidn_filter);
	if (r->oidn_filter_fast)
		oidnReleaseFilter(r->oidn_filter_fast);

	r->oidn_buffer = oidnNewBuffer(r->oidn_device, size);
	r->oidn_albedo = oidnNewBuffer(r->oidn_device, size);
	r->oidn_normal = oidnNewBuffer(r->oidn_device, size);

	r->oidn_filter = oidnNewFilter(r->oidn_device, "RT");
	oidnSetFilterImage(r->oidn_filter, "color", r->oidn_buffer, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter, "albedo", r->oidn_albedo, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter, "normal", r->oidn_normal, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter, "output", r->oidn_buffer, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterBool(r->oidn_filter, "hdr", true);
	oidnSetFilterBool(r->oidn_filter, "cleanAux", true);
	oidnCommitFilter(r->oidn_filter);

	r->oidn_filter_fast = oidnNewFilter(r->oidn_device, "RT");
	oidnSetFilterImage(r->oidn_filter_fast, "color", r->oidn_buffer, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter_fast, "albedo", r->oidn_albedo, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter_fast, "normal", r->oidn_normal, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterImage(r->oidn_filter_fast, "output", r->oidn_buffer, OIDN_FORMAT_FLOAT3, r->width, r->height, 0, sizeof(t_vec3), 0);
	oidnSetFilterBool(r->oidn_filter_fast, "hdr", true);
	oidnSetFilterBool(r->oidn_filter_fast, "cleanAux", false);
	oidnCommitFilter(r->oidn_filter_fast);
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
