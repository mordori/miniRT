#include "camera.h"
#include "editing.h"
#include "input.h"
#include "rendering.h"

static inline void flag_update(t_context* ctx, bool* update);

void process_input(t_context* ctx, bool* update) {
	if (ctx->renderer.mode != RENDERED) {
		while (ctx->renderer.threads_running)
			pthread_cond_wait(&ctx->renderer.cond, &ctx->renderer.mutex);
	}

	t_vec2i mouseDelta = get_mouse_delta(ctx);
	bool dirty = false;
	dirty |= config_camera(ctx);
	dirty |= control_camera(ctx, mouseDelta);
	dirty |= edit_object(ctx, mouseDelta);
	dirty |= camera_movement(ctx);
	dirty |= rotate_skydome(ctx);
	if (dirty)
		flag_update(ctx, update);
}

static inline void flag_update(t_context* ctx, bool* update) {
	*update = true;
	update_camera(ctx, &ctx->scene.cam);
	if (ctx->renderer.mode == RENDERED)
		atomic_store(&ctx->renderer.render_cancel, true);
}
