#include "editing.h"

void	obj_translate(t_context *ctx, t_vec3 delta)
{
	ctx->editor.selected_obj->transform.pos = vec3_add(ctx->editor.selected_obj->transform.pos, delta);
}
