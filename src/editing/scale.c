#include "editing.h"

void	obj_scale(t_context *ctx, t_vec3 axis_delta)
{
	ctx->editor.selected_obj->transform.scale = vec3_mul(ctx->editor.selected_obj->transform.scale, axis_delta);
	ctx->editor.selected_obj->transform.scale.v = _mm_max_ps(ctx->editor.selected_obj->transform.scale.v, vec4_3(vec3_n(0.01f), 1.0f).v);
}
