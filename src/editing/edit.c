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
	if (keydata.key == MLX_KEY_G || keydata.key == MLX_KEY_R || keydata.key == MLX_KEY_S)
	{
		if (keydata.key == MLX_KEY_G)
			ctx->editor.mode = EDIT_TRANSLATE;
		if (keydata.key == MLX_KEY_R)
			ctx->editor.mode = EDIT_ROTATE;
		if (keydata.key == MLX_KEY_S)
			ctx->editor.mode = EDIT_SCALE;
		ctx->editor.axis_primary = ctx->scene.cam.right;
		ctx->editor.axis_secondary = ctx->scene.cam.up;
		begin_edit_action(ctx);
	}
	if (ctx->editor.mode != EDIT_DEFAULT)
		set_axis_constraints(ctx, keydata);
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
	t_vec3		axis_delta;
	float		speed;
	float		dist;
	float		scale_avg;
	float		magnitude;
	float		right_align;
	float		up_align;
	bool		is_single_constraint;

	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (delta.x || delta.y)
		{
			if (ctx->editor.mode == EDIT_TRANSLATE)
			{
				dist = vec3_dist(ctx->scene.cam.transform.pos, ctx->editor.selected_obj->transform.pos);
				dist = fmaxf(dist, 1.0f);
				speed = ctx->mlx->delta_time * 60.0f * SENS_TRANSLATE * dist * (50.0f / ctx->scene.cam.focal_len_mm);
			}
			else if (ctx->editor.mode == EDIT_ROTATE)
				speed = ctx->mlx->delta_time * 60.0f * SENS_ROTATE;
			else
			{
				scale_avg = (\
ctx->editor.selected_obj->transform.scale.x + \
ctx->editor.selected_obj->transform.scale.y + \
ctx->editor.selected_obj->transform.scale.z) * 0.3333f;
				speed = ctx->mlx->delta_time * 60.0f * SENS_SCALE * (scale_avg + 0.1f);
			}
			is_single_constraint = (ctx->editor.has_constraints && \
ctx->editor.axis_secondary.x == 0.0f && \
ctx->editor.axis_secondary.y == 0.0f && \
ctx->editor.axis_secondary.z == 0.0f);
			if (ctx->editor.mode == EDIT_TRANSLATE)
			{
				if (is_single_constraint)
				{
					right_align = vec3_dot(ctx->scene.cam.right, ctx->editor.axis_primary);
					up_align = vec3_dot(ctx->scene.cam.up, ctx->editor.axis_primary);
					if (fabsf(right_align) < 0.1f && fabsf(up_align) < 0.1f)
						magnitude = (float)(delta.x - delta.y) * speed;
					else
						magnitude = ((float)delta.x * right_align - (float)delta.y * up_align) * speed;
					axis_delta = vec3_scale(ctx->editor.axis_primary, magnitude);
				}
				else
				{
					axis_delta = vec3_add(\
vec3_scale(ctx->editor.axis_primary, delta.x * speed), \
vec3_scale(ctx->editor.axis_secondary, -delta.y * speed));
				}
				obj_translate(ctx, axis_delta);
			}
			// else if (ctx->editor.mode == EDIT_ROTATE)
			// 	obj_rotate(ctx, axis_delta);
			// else if (ctx->editor.mode == EDIT_SCALE)
			// {
			// 	if (!ctx->editor.has_constraints)
			// 		axis_delta = vec3_n((float)(delta.x - delta.y) * speed);
			// 	obj_scale(ctx, axis_delta);
			// }
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
