#include "input.h"
#include "camera.h"

bool	process_input(t_context *ctx)
{
	bool	dirty;

	dirty = false;
	if (control_camera(ctx))
	{
		dirty = true;
		update_camera(ctx);
	}
	return (dirty);
}
