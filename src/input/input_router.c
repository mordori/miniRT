#include "input.h"
#include "camera.h"
#include "utils.h"
#include "rendering.h"
#include "editing.h"

static inline void	flag_update(t_context *ctx, bool *update);

void	process_input(t_context *ctx, bool *update)
{
	bool		dirty;
	t_vec2i		delta;

	if (ctx->renderer.mode != RENDERED)
	{
		while (ctx->renderer.threads_running)
			pthread_cond_wait(&ctx->renderer.cond, &ctx->renderer.mutex);
	}
	dirty = false;
	delta = get_mouse_delta(ctx);
	if (config_camera(ctx))
		dirty = true;
	if (control_camera(ctx, delta))
		dirty = true;
	if (edit_object(ctx, delta))
		dirty = true;
	if (camera_movement(ctx))
		dirty = true;
	if (rotate_skydome(ctx))
		dirty = true;
	if (dirty)
		flag_update(ctx, update);
}

static inline void	flag_update(t_context *ctx, bool *update)
{
	*update = true;
	update_camera(ctx, &ctx->scene.cam);
	if (ctx->renderer.mode == RENDERED)
		atomic_store(&ctx->renderer.render_cancel, true);
}
