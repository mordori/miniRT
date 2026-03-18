#include "editing.h"
#include "utils.h"
#include "input.h"
#include "objects.h"

static inline void	begin_edit_action(t_context *ctx);
static inline void	end_edit_action(t_context *ctx);
static inline void	edit_action(t_context *ctx, t_vec2i delta);

void	config_editor(t_context *ctx, mlx_key_data_t keydata)
{
	if (!ctx->editor.selected_obj || keydata.action != MLX_RELEASE)
		return ;
	if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_E || keydata.key == MLX_KEY_R)
	{
		if (keydata.key == MLX_KEY_W)
			ctx->editor.mode = EDIT_TRANSLATE;
		if (keydata.key == MLX_KEY_E)
			ctx->editor.mode = EDIT_ROTATE;
		if (keydata.key == MLX_KEY_R)
			ctx->editor.mode = EDIT_SCALE;
		ctx->editor.axis_primary = ctx->scene.cam.right;
		ctx->editor.axis_secondary = ctx->scene.cam.up;
		begin_edit_action(ctx);
	}
	if (ctx->editor.mode != EDIT_DEFAULT)
	{
		if (keydata.key == MLX_KEY_X || keydata.key == MLX_KEY_Y || keydata.key == MLX_KEY_Z)
			set_axis_constraints(ctx, keydata);
	}
}

bool	edit_object(t_context *ctx, t_vec2i delta)
{
	if (ctx->scene.cam.state != CAM_DEFAULT)
		return (false);
	if (ctx->editor.mode == EDIT_DEFAULT || !ctx->editor.selected_obj)
		return (false);
	edit_action(ctx, delta);
	return (delta.x != 0 || delta.y != 0);
}

static inline void	edit_action(t_context *ctx, t_vec2i delta)
{
	t_vec3		primary_delta;
	t_vec3		secondary_delta;
	t_vec3		axis_delta;
	float		speed;

	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (delta.x || delta.y)
		{
			speed = ctx->mlx->delta_time * 60.0f * SENS_EDIT;
			primary_delta = vec3_scale(ctx->editor.axis_primary, (float)delta.x * speed);
			secondary_delta = vec3_scale(ctx->editor.axis_secondary, (float)-delta.y * speed);
			axis_delta = vec3_add(primary_delta, secondary_delta);
			if (ctx->editor.mode == EDIT_TRANSLATE)
				obj_translate(ctx, axis_delta);
			else if (ctx->editor.mode == EDIT_ROTATE)
				obj_rotate(ctx, axis_delta);
			else if (ctx->editor.mode == EDIT_SCALE)
				obj_scale(ctx, axis_delta);
			update_transform(&ctx->editor.selected_obj->transform);
		}
	}
}

static inline void	begin_edit_action(t_context *ctx)
{
	mlx_set_cursor_mode(ctx->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos_orig.x, &ctx->mouse.pos_orig.y);
	ctx->mouse.pos = ctx->mouse.pos_orig;
	ctx->mouse.pos_prev = ctx->mouse.pos_orig;
}

static inline void	end_edit_action(t_context *ctx)
{
	ctx->editor.mode = EDIT_DEFAULT;
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
	end_edit_action(ctx);
}
