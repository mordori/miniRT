#include "editing.h"

void	obj_translate(\
t_context *ctx, float magnitude, t_vec2i delta, float speed)
{
	t_vec3		axis;

	if (ctx->editor.constraints == 1)
		axis = vec3_scale(ctx->editor.axis_primary, magnitude);
	else
	{
		axis = vec3_add(\
vec3_scale(ctx->editor.axis_primary, delta.x * speed), \
vec3_scale(ctx->editor.axis_secondary, -delta.y * speed));
	}
	ctx->editor.selected_obj->transform.pos = \
vec3_add(ctx->editor.selected_obj->transform.pos, axis);
}
