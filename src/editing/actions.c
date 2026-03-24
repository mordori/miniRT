#include "editing.h"
#include "lights.h"
#include "utils.h"

void	edit_action(t_context *ctx, t_vec2i delta)
{
	float		speed;
	float		magnitude;

	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (delta.x || delta.y)
		{
			speed = eval_speed(ctx);
			magnitude = eval_magnitude(ctx, delta, speed);
			if (ctx->editor.mode == EDIT_TRANSLATE)
				obj_translate(ctx, magnitude, delta, speed);
			else if (ctx->editor.mode == EDIT_ROTATE)
				obj_rotate(ctx, magnitude);
			else if (ctx->editor.mode == EDIT_SCALE)
				obj_scale(ctx, magnitude);
			update_transform(&ctx->editor.selected_obj->transform);
			update_lights_radii(ctx);
		}
	}
}

void	begin_edit_action(t_context *ctx)
{
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos_orig.x, &ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

void	end_edit_action(t_context *ctx)
{
	ctx->editor.mode = EDIT_DEFAULT;
	ctx->editor.constraint_axis = AXIS_DEFAULT;
	ctx->editor.constraints = 0;
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
	mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos_orig.x, ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

void	apply_edit_action(t_context *ctx)
{
	end_edit_action(ctx);
}

void	cancel_edit_action(t_context *ctx)
{
	ctx->editor.selected_obj->transform = ctx->editor.orig_transform;
	update_transform(&ctx->editor.selected_obj->transform);
	end_edit_action(ctx);
}
