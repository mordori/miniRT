#include "editing.h"

void	obj_rotate(t_context *ctx, t_vec3 axis_delta)
{
	ctx->editor.selected_obj->transform.rot = vec3_add(ctx->editor.selected_obj->transform.rot, axis_delta);
}
