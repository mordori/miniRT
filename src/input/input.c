#include "input.h"
#include "camera.h"

void	process_input(t_context *ctx)
{
	if (control_camera(ctx))
		update_camera(ctx);
	// if ()
}
