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
	if (rotate_skydome(ctx))
		dirty = true;
	if (dirty)
	{
		if (ctx->renderer.mode == RENDER_REFINE)
			atomic_store(&ctx->renderer.render_cancel, true);
		*update = true;
	}
}
