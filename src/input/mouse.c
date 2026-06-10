#include "defines.h"
#include "input.h"

t_vec2i get_mouse_delta(t_context* ctx) {
	if (ctx->editor.mode == EDIT_DEFAULT && ctx->scene.cam.state == CAM_DEFAULT)
		return (t_vec2i){ 0, 0 };
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos.x, &ctx->mouse.pos.y);
	t_vec2i delta = vec2i_sub(ctx->mouse.pos, ctx->mouse.pos_prev);
	ctx->mouse.pos_prev = ctx->mouse.pos;
	return delta;
}
