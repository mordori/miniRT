#include "input.h"
#include "camera.h"
#include "utils.h"
#include "rendering.h"

void	process_input(t_context *ctx, bool *update)
{
	bool		dirty;

	dirty = false;
	if (control_camera(ctx))
		dirty = true;
	if (camera_movement(ctx))
		dirty = true;
	if (rotate_skydome(ctx))
		dirty = true;
	if (is_key_down(ctx))
		dirty = true;
	if (dirty)
	{
		*update = true;
		update_camera(ctx, &ctx->scene.cam);
		if (ctx->renderer.mode == RENDERED)
			atomic_store(&ctx->renderer.render_cancel, true);
	}
}
