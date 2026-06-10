#include <stdint.h>

#include "editing.h"
#include "lib_math.h"
#include "rendering.h"
#include "utils.h"

static inline void* render_routine(void* arg);
static inline void render_tile(const t_context* ctx, t_vec3* buf, uint32_t tile_id);
static inline void render_pixel(const t_context* ctx, t_pixel* pixel);
static inline void sample_pixel(const t_context* ctx, t_pixel* pixel);
static inline bool get_thread_status(t_renderer* r, uint32_t* tile_id);
static inline bool set_render_mode(t_context* ctx, t_renderer* r, mlx_key_data_t keydata);
// static inline void set_samples(t_context* ctx, mlx_key_data_t keydata);
// static inline bool set_bounces(t_context* ctx, mlx_key_data_t keydata);

bool init_renderer(t_context* ctx) {
	t_renderer* r = &ctx->renderer;

	r->init_mutex = !pthread_mutex_init(&r->mutex, NULL);
	r->init_cond = !pthread_cond_init(&r->cond, NULL);

	r->threads_amount = (int32_t)sysconf(_SC_NPROCESSORS_ONLN);
	if (r->threads_amount == ERROR)
		r->threads_amount = THREADS_DFL;
	r->threads_amount *= 2;
	r->threads = malloc(sizeof(*r->threads) * r->threads_amount);
	if (!r->init_mutex || !r->init_cond || !r->threads)
		fatal_error(ctx, errors(ERR_RENINIT), __FILE__, __LINE__);

	r->oidn_device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
	oidnCommitDevice(r->oidn_device);

	r->active = true;
	while (r->threads_init < r->threads_amount) {
		if (pthread_create(&r->threads[r->threads_init], NULL, render_routine, ctx)) {
			pthread_mutex_lock(&r->mutex);
			r->active = false;
			pthread_mutex_unlock(&r->mutex);
			break;
		}
		++r->threads_init;
	}
	return r->threads_init == r->threads_amount;
}

static inline void* render_routine(void* arg) {
	t_context* ctx = (t_context*)arg;
	t_renderer* r = &ctx->renderer;

	pthread_mutex_lock(&r->mutex);
	while (true) {
		while (r->active && (r->resize_pending || r->tile_index >= r->tiles_total))
			pthread_cond_wait(&r->cond, &r->mutex);

		uint32_t tile_id;
		if (!get_thread_status(r, &tile_id))
			break;

		pthread_mutex_unlock(&r->mutex);
		render_tile(ctx, r->buffer, tile_id);
		pthread_mutex_lock(&r->mutex);

		--r->threads_running;
		if (r->threads_running == 0 && (r->resize_pending || r->tile_index >= r->tiles_total)) {
			r->frame_complete = true;
			pthread_cond_broadcast(&r->cond);
		}
	}
	return NULL;
}

static inline void render_tile(const t_context* ctx, t_vec3* buf, uint32_t tile_id) {
	t_uint2 start = { //
		.x = (tile_id % ctx->renderer.tiles.x) * TILE_SIZE,
		.y = (tile_id / ctx->renderer.tiles.x) * TILE_SIZE
	};
	t_uint2 end = { //
		.x = ft_uint_min(start.x + TILE_SIZE, ctx->renderer.width),
		.y = ft_uint_min(start.y + TILE_SIZE, ctx->renderer.height)
	};

	t_pixel pixel = { .y = start.y };
	uint32_t width = ctx->renderer.width;
	buf = __builtin_assume_aligned(buf, 64);

	while (pixel.y < end.y && !atomic_load(&ctx->renderer.render_cancel)) {
		pixel.color = &buf[pixel.y * width + start.x];
		pixel.x = start.x;
		while (pixel.x < end.x && !atomic_load(&ctx->renderer.render_cancel)) {
			render_pixel(ctx, &pixel);
			++pixel.color;
			++pixel.x;
		}
		++pixel.y;
	}
}

static inline void render_pixel(const t_context* ctx, t_pixel* pixel) {
	const t_renderer* r = &ctx->renderer;

	uint32_t seed = hash_lowerbias32((pixel->y * r->width + pixel->x) ^ hash_lowerbias32(r->frame));
	if (seed == 0)
		seed = 1;
	pixel->seed = &seed;
	pixel->frame = r->frame;

	sample_pixel(ctx, pixel);
	t_vec3 color = trace_path(ctx, pixel, r->mode, r->ray_bounces);

	if (r->frame == 1)
		*pixel->color = color;
	else
		*pixel->color = vec3_add(*pixel->color, color);
}

static inline void sample_pixel(const t_context* ctx, t_pixel* pixel) {
	const t_renderer* r = &ctx->renderer;

	if (r->mode == RENDERED && r->render_samples > 1) {
		t_vec2 random_01 = { //
			.u = static_blue_noise(&ctx->tex_bn, pixel, BN_PX_U),
			.v = static_blue_noise(&ctx->tex_bn, pixel, BN_PX_V)
		};
		t_vec2 uv = r2_sequence(r->frame, random_01);
		pixel->u = (float)pixel->x + uv.u;
		pixel->v = (float)pixel->y + uv.v;
	} else {
		pixel->u = (float)pixel->x + 0.5f;
		pixel->v = (float)pixel->y + 0.5f;
	}
}

static inline bool get_thread_status(t_renderer* r, uint32_t* tile_id) {
	if (!r->active) {
		pthread_mutex_unlock(&r->mutex);
		return false;
	}

	*tile_id = r->tile_index++;
	++r->threads_running;
	return true;
}

void start_render(t_renderer* r, const t_camera* cam) {
	pthread_mutex_lock(&r->mutex);
	r->tiles.x = (r->width + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles.y = (r->height + TILE_SIZE - 1) / TILE_SIZE;
	r->tiles_total = r->tiles.x * r->tiles.y;
	r->tile_index = 0;
	r->cam = *cam;
	r->ray_bounces = r->render_bounces;
	r->frame = 1;
	r->render_time = time_now();
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
}

void stop_render(t_renderer* r) {
	pthread_mutex_lock(&r->mutex);
	r->active = false;
	r->resize_pending = false;
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->mutex);
}

void cancel_render(t_renderer* r) {
	r->tile_index = r->tiles_total;
	pthread_cond_broadcast(&r->cond);
	while (r->threads_running)
		pthread_cond_wait(&r->cond, &r->mutex);
	atomic_store(&r->render_cancel, false);
	r->frame_complete = false;
	r->frame = 1;
	r->render_time = time_now();
	r->blit_time = 0;
}

void set_mode_preview(t_context* ctx, t_renderer* r, bool* update) {
	if (r->mode == RENDERED)
		r->mode = PREVIEW;
	if (r->mode == SOLID)
		memset(ctx->editor.selection_mask, 0, sizeof(float) * ctx->renderer.pixels);
	r->cam = ctx->scene.cam;
	r->ray_bounces = PREVIEW_BOUNCES;
	r->frame = 1;
	r->tile_index = 0;
	*update = false;
	r->render_time = time_now();
	pthread_cond_broadcast(&r->cond);
}

void set_mode_rendered(t_renderer* r) {
	if (r->mode != RENDERED) {
		r->blit_time = 0;
		r->render_time = time_now();
		r->frame = 1;
	}
	r->mode = RENDERED;
	r->ray_bounces = r->render_bounces;
	r->tile_index = 0;
	pthread_cond_broadcast(&r->cond);
}

bool config_renderer(t_context* ctx, mlx_key_data_t keydata) {
	bool dirty = false;
	dirty |= set_render_mode(ctx, &ctx->renderer, keydata);
	return dirty;
}

static inline bool set_render_mode(t_context* ctx, t_renderer* r, mlx_key_data_t keydata) {
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS) {
		while (r->threads_running)
			pthread_cond_wait(&r->cond, &r->mutex);

		if (r->mode != SOLID) {
			r->mode = SOLID;
		} else {
			if (ctx->editor.selected_obj)
				reset_editor(ctx, r);
			else if (ctx->editor.mode != EDIT_DEFAULT)
				end_edit_action(ctx);
			ctx->editor.mode = EDIT_DEFAULT;
			r->mode = RENDERED;
		}
		return true;
	}
	return false;
}
