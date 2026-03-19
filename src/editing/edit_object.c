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
		if (ctx->editor.mode == EDIT_DEFAULT)
			ctx->editor.orig_transform = ctx->editor.selected_obj->transform;
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
	float		magnitude;
	float		right_align;
	float		up_align;
	bool		is_single_constraint;
	t_mat4		*m;
	t_vec3		axis_local;

	if (delta.x > -300 && delta.x < 300 && delta.y > -300 && delta.y < 300)
	{
		if (delta.x || delta.y)
		{
			if (ctx->editor.mode == EDIT_TRANSLATE)
			{
				dist = vec3_dist(ctx->scene.cam.transform.pos, ctx->editor.selected_obj->transform.pos);
				dist = fmaxf(dist, 1.0f);
				speed = ctx->mlx->delta_time * 60.0f * SENS_TRANSLATE * dist * (14.0f / ctx->scene.cam.focal_len_mm);
			}
			else if (ctx->editor.mode == EDIT_ROTATE)
				speed = ctx->mlx->delta_time * 60.0f * SENS_ROTATE;
			else
				speed = ctx->mlx->delta_time * 60.0f * SENS_SCALE;
			is_single_constraint = (ctx->editor.has_constraints && \
ctx->editor.axis_secondary.x == 0.0f && \
ctx->editor.axis_secondary.y == 0.0f && \
ctx->editor.axis_secondary.z == 0.0f);
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
				magnitude = (float)(delta.x - delta.y) * speed;
				axis_delta = vec3_add(\
vec3_scale(ctx->editor.axis_primary, delta.x * speed), \
vec3_scale(ctx->editor.axis_secondary, -delta.y * speed));
			}
			if (ctx->editor.mode == EDIT_TRANSLATE)
				obj_translate(ctx, axis_delta);
			else if (ctx->editor.mode == EDIT_ROTATE)
			{
				if (!ctx->editor.has_constraints)
					axis_delta = ctx->scene.cam.forward;
				else if ( is_single_constraint)
					axis_delta = ctx->editor.axis_primary;
				else
				{
					if (ctx->editor.axis_primary.z == 0.0f && ctx->editor.axis_secondary.z == 0.0f)
						axis_delta = g_forward;
					else if (ctx->editor.axis_primary.y == 0.0f && ctx->editor.axis_secondary.y == 0.0f)
						axis_delta = g_up;
					else
						axis_delta = g_right;
				}
				obj_rotate(ctx, vec3_scale(axis_delta, magnitude));
			}
			else if (ctx->editor.mode == EDIT_SCALE)
			{
				if (!ctx->editor.has_constraints)
					axis_delta = vec3_n(expf(magnitude));
				else
				{
					m = &ctx->editor.selected_obj->transform.object_to_world;
					axis_local.x = vec3_dot(ctx->editor.axis_primary, vec3(m->m[0][0], m->m[1][0], m->m[2][0]));
					axis_local.y = vec3_dot(ctx->editor.axis_primary, vec3(m->m[0][1], m->m[1][1], m->m[2][1]));
					axis_local.z = vec3_dot(ctx->editor.axis_primary, vec3(m->m[0][2], m->m[1][2], m->m[2][2]));
					axis_delta.x = expf(fabsf(axis_local.x) * magnitude);
					axis_delta.y = expf(fabsf(axis_local.y) * magnitude);
					axis_delta.z = expf(fabsf(axis_local.z) * magnitude);
				}
				obj_scale(ctx, axis_delta);
			}
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
	ctx->editor.selected_obj->transform = ctx->editor.orig_transform;
	update_transform(&ctx->editor.selected_obj->transform);
	end_edit_action(ctx);
}
