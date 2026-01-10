#include "input.h"
#include "camera.h"

bool	process_input(t_context *ctx)
{
	bool	dirty;

	dirty = control_camera(ctx);
	if (dirty)
		update_camera(ctx);
	return (dirty);
}
