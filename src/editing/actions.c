#include "MLX42.h"
#include "defines.h"
#include "editing.h"
#include "lights.h"
#include "objects.h"
#include "scene.h"
#include "utils.h"

void edit_action(t_context* ctx, t_vec2i delta) {
	if (delta.x || delta.y) {
		float speed = eval_speed(ctx);
		float magnitude = eval_magnitude(ctx, delta, speed);
		if (ctx->editor.mode == EDIT_TRANSLATE)
			obj_translate(ctx, magnitude, delta, speed);
		else if (ctx->editor.mode == EDIT_ROTATE)
			obj_rotate(ctx, magnitude);
		else if (ctx->editor.mode == EDIT_SCALE)
			obj_scale(ctx, magnitude);
		update_transform(&ctx->editor.selected_obj->transform);
		update_bounds(ctx->editor.selected_obj);
		update_light_radius(ctx);
	}
}

void begin_edit_action(t_context* ctx, t_edit_mode mode) {
	if (mode != EDIT_SCALE) {
		if (is_wsl())
			mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
		else
			mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_DISABLED);
	}
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos_orig.x, &ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
	ctx->editor.axis_primary = ctx->scene.cam.right;
	ctx->editor.axis_secondary = ctx->scene.cam.up;
	ctx->editor.selection_time = engine_time();
}

void end_edit_action(t_context* ctx) {
	ctx->editor.constraint_axis = AXIS_DEFAULT;
	ctx->editor.constraints = 0;
	if (ctx->editor.mode != EDIT_SCALE) {
		mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_NORMAL);
		if (ctx->editor.mode != EDIT_TRANSLATE) {
			mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos_orig.x, ctx->mouse.pos_orig.y);
			ctx->mouse.pos = ctx->mouse.pos_orig;
			ctx->mouse.pos_prev = ctx->mouse.pos_orig;
		}
	}
	ctx->editor.mode = EDIT_DEFAULT;
	ctx->editor.selection_time = engine_time();
}

void apply_edit_action(t_context* ctx) {
	end_edit_action(ctx);
}

void cancel_edit_action(t_context* ctx) {
	ctx->editor.selected_obj->transform = ctx->editor.orig_transform;
	update_transform(&ctx->editor.selected_obj->transform);
	update_bounds(ctx->editor.selected_obj);
	update_light_radius(ctx);
	end_edit_action(ctx);
}
