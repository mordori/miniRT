#include <stdint.h>

#include "MLX42.h"
#include "defines.h"
#include "input.h"
#include "lib_math.h"
#include "rendering.h"
#include "ui.h"
#include "utils.h"

static inline void process_frame(t_context* ctx, t_renderer* r);
static inline void set_renderer_state(t_context* ctx, t_renderer* r, bool* update);
static inline bool is_active(t_context* ctx);
static inline void copy_frame_buffer_rendered(const t_context* ctx, t_vec3* buf, uint32_t* pixels, t_pixel* pixel);
static inline void copy_frame_buffer_preview(const t_context* ctx, const uint32_t width, t_vec3* buf, uint32_t* pixels);
static inline void print_render_status(t_context* ctx, t_renderer* r);
static inline void limit_polling_rate(t_renderer* r);
static inline uint32_t color_wave(uint32_t c1, uint32_t c2, float speed);

void frame_loop(void* param) {
	t_context* ctx = (t_context*)param;
	t_renderer* r = &ctx->renderer;
	static bool update;

	pthread_mutex_lock(&r->mutex);
	process_input(ctx, &update);
	if (atomic_load(&r->render_cancel) || r->resize_pending) {
		cancel_render(r);
		update = true;
	} else if (r->frame_complete && !r->resize_pending) {
		process_frame(ctx, r);
	}
	set_renderer_state(ctx, r, &update);
	pthread_mutex_unlock(&r->mutex);

	update_ui(ctx);
	limit_polling_rate(r);
}

static inline void set_renderer_state(t_context* ctx, t_renderer* r, bool* update) {
	if (r->resize_pending) {
		pthread_mutex_unlock(&r->mutex);
		if (ctx->resize_time != 0 && timer(ctx->resize_time, 250))
			resize_window(ctx);
		else
			pthread_mutex_lock(&r->mutex);
		return;
	}
	if (r->threads_running || r->tile_index < r->tiles_total)
		return;

	if (is_active(ctx) || (r->mode == SOLID && *update)) {
		set_mode_preview(ctx, r, update);
	} else if (r->mode == PREVIEW) {
		*update = false;
		set_mode_rendered(r);
	} else if (*update) {
		*update = false;
		if (r->mode == RENDERED && r->frame > 1) {
			r->frame = 1;
			r->render_time = time_now();
			r->blit_time = 0;
		}
		set_mode_rendered(r);
	} else if (r->mode != SOLID && r->frame <= r->render_samples) {
		set_mode_rendered(r);
	}
}

static inline bool is_active(t_context* ctx) {
	if (ctx->scene.cam.state != CAM_DEFAULT || ctx->editor.mode != EDIT_DEFAULT)
		return true;
	if (ctx->renderer.mode == SOLID)
		return false;

	mlx_t* m = ctx->mlx;
	if (mlx_is_key_down(m, KEY_FORWARD) || mlx_is_key_down(m, KEY_BACK) || mlx_is_key_down(m, KEY_RIGHT) || mlx_is_key_down(m, KEY_LEFT) ||
		mlx_is_key_down(m, KEY_UP) || mlx_is_key_down(m, KEY_DOWN)) {
		return true;
	}
	if (mlx_is_key_down(m, MLX_KEY_L) || mlx_is_key_down(m, MLX_KEY_K) ||
		(ctx->scene.env.skydome.pixels && (mlx_is_key_down(m, MLX_KEY_COMMA) || mlx_is_key_down(m, MLX_KEY_PERIOD)))) {
		return true;
	}
	if (mlx_is_key_down(m, MLX_KEY_LEFT) || mlx_is_key_down(m, MLX_KEY_RIGHT) || mlx_is_key_down(m, MLX_KEY_UP) ||
		mlx_is_key_down(m, MLX_KEY_DOWN)) {
		return true;
	}
	return false;
}

void blit(const t_context* ctx, const t_renderer* r, bool is_denoised) {
	uint32_t* pixels = (uint32_t*)((void*)ctx->img->pixels);
	t_vec3* buf = __builtin_assume_aligned(is_denoised ? r->denoise_buffer : r->buffer, 64);
	t_pixel pixel = { .frame = r->frame };
	if (pixel.frame <= 1 || is_denoised) {
		pixel.scale = 1.0f;
		pixel.frame = 1;
	} else {
		pixel.scale = 1.0f / (float)pixel.frame;
	}
	if (r->mode == RENDERED)
		copy_frame_buffer_rendered(ctx, buf, pixels, &pixel);
	else
		copy_frame_buffer_preview(ctx, r->width, buf, pixels);
}

static inline void process_frame(t_context* ctx, t_renderer* r) {
	static char buf[128];

	print_render_status(ctx, r);
	if (r->mode != RENDERED || r->frame < 8 || (r->frame < 16 && (r->frame & 1)) || r->frame == 32 || r->frame == 48 || r->frame == 64 ||
		r->frame == 80 || (time_now() > r->blit_time + 5000 || r->frame >= r->render_samples)) {
		uint32_t render_time = time_now() - r->render_time;
		bool is_final = r->frame >= r->render_samples && r->mode == RENDERED;
		if (is_final)
			denoise(r);
		blit(ctx, r, is_final);
		r->blit_time = time_now();
		if (is_final) {
			snprintf(buf, sizeof(buf), "\r\033[K\033[1;32mDone!   Frames: %d   Render time: %.1fs\033[0m", r->frame, render_time / 1000.0f);
			try_write(ctx, STDOUT_FILENO, buf);
		}
	}
	r->frame_complete = false;
	++r->frame;
}

static inline void copy_frame_buffer_rendered(const t_context* ctx, t_vec3* buf, uint32_t* pixels, t_pixel* pixel) {
	uint32_t limit = ctx->renderer.pixels;
	uint32_t width = ctx->renderer.width;
	uint32_t i = 0;
	while (i < limit) {
		t_vec3 color = vec3_scale(buf[i], pixel->scale);
		color = post_process(ctx, pixel, color);
		pixels[i++] = vec3_to_uint32(color);
		if (pixel->x++ == width) {
			pixel->x = 0;
			pixel->y++;
		}
	}
}

static inline void copy_frame_buffer_preview(const t_context* ctx, const uint32_t width, t_vec3* buf, uint32_t* pixels) {
	float* m = ctx->editor.selection_mask;
	uint32_t limit = ctx->renderer.pixels;
	uint32_t mask;
	uint32_t edge_color = color_wave(0xFF007BFF, 0xFFFFFFFF, 10.0f);
	uint32_t i = 0;
	while (i < limit) {
		if (m[i] > 0.0f) {
			mask = 0u -
				(((i % width && m[i - 1] < 0.0f && fabsf(m[i - 1]) > m[i] - 0.05f) ||
					((i + 1) % width && m[i + 1] < 0.0f && fabsf(m[i + 1]) > m[i] - 0.05f) ||
					(i >= width && m[i - width] < 0.0f && fabsf(m[i - width]) > m[i] - 0.05f) ||
					(i + width < limit && m[i + width] < 0.0f && fabsf(m[i + width]) > m[i] - 0.05f)));
		} else {
			mask = 0u;
		}
		pixels[i] = (edge_color & mask) | (vec3_to_uint32(post_process_preview(ctx, buf[i])) & ~mask);
		++i;
	}
}

static inline void print_render_status(t_context* ctx, t_renderer* r) {
	const int bar_width = 20;
	static char buf[128];
	static char bar[32];

	int hashes = (r->frame * bar_width) / r->render_samples;
	if (hashes > bar_width)
		hashes = bar_width;
	int i = 0;
	while (i < bar_width) {
		if (i < hashes)
			bar[i] = '#';
		else
			bar[i] = ' ';
		++i;
	}
	bar[bar_width] = '\0';
	snprintf(buf, sizeof(buf), "\r\033[K\033[1;33mRendering...   [%s] [%u/%u]\033[0m", bar, r->frame, r->render_samples);
	try_write(ctx, STDOUT_FILENO, buf);
}

static inline void limit_polling_rate(t_renderer* r) {
	static uint32_t last_frame_time;

	if (r->frame < r->render_samples || r->mode != RENDERED)
		return;

	wait_until(last_frame_time + 17);
	last_frame_time = time_now();
}

static inline uint32_t color_wave(uint32_t c1, uint32_t c2, float speed) {
	float time = engine_time() / 1000.0f;
	float wave = sinf(time * speed);
	float t = (wave + 1.0f) * 0.5f;
	return vec3_to_uint32(lerp_color(c1, c2, t));
}

void denoise(t_renderer* r) {
	uint32_t limit = r->pixels;
	float scale = 1.0f / (float)r->frame;
	size_t buffer_bytes = r->width * r->height * sizeof(t_vec3);

	for (uint32_t i = 0; i < limit; ++i)
		r->denoise_buffer[i] = vec3_scale(r->albedo_buffer[i], scale);
	oidnWriteBuffer(r->oidn_albedo, 0, buffer_bytes, r->denoise_buffer);

	for (uint32_t i = 0; i < limit; ++i)
		r->denoise_buffer[i] = vec3_normalize(vec3_scale(r->normal_buffer[i], scale));
	oidnWriteBuffer(r->oidn_normal, 0, buffer_bytes, r->denoise_buffer);

	for (uint32_t i = 0; i < limit; ++i)
		r->denoise_buffer[i] = vec3_scale(r->buffer[i], scale);
	oidnWriteBuffer(r->oidn_buffer, 0, buffer_bytes, r->denoise_buffer);

	if (r->frame <= 4)
		oidnExecuteFilter(r->oidn_filter_fast);
	else
		oidnExecuteFilter(r->oidn_filter);
	oidnReadBuffer(r->oidn_buffer, 0, buffer_bytes, r->denoise_buffer);
}
