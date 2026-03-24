#include "editing.h"
#include "utils.h"
#include "input.h"
#include "objects.h"

static inline void	begin_edit_action(t_context *ctx);
static inline void	end_edit_action(t_context *ctx);
static inline void	edit_action(t_context *ctx, t_vec2i delta);

void	config_editor(t_context *ctx, mlx_key_data_t keydata)
{
	const t_axis	prev_constraints = ctx->editor.constraints;

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
		begin_edit_action(ctx);
	}
	if (ctx->editor.mode != EDIT_DEFAULT)
		set_axis_constraints(ctx, keydata);
	if (ctx->editor.constraints != prev_constraints)
	{
		ctx->editor.selected_obj->transform = ctx->editor.orig_transform;
		update_transform(&ctx->editor.selected_obj->transform);
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
	t_vec3		axis_delta;
	float		speed;
	float		dist;
	float		magnitude;
	float		right_align;
	float		up_align;
	bool		is_single_constraint;
	t_mat4		*m;
	t_vec3		local_x;
	t_vec3		local_y;
	t_vec3		local_z;
	t_vec2		w_x;
	t_vec2		w_y;
	t_vec2		w_z;
	t_vec2		obj_screen_pos;
	t_vec2		dir_mouse_to_obj_screen;
	float		dist_mouse_to_obj_screen;

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
			is_single_constraint = (ctx->editor.constraints != AXIS_DEFAULT && \
ctx->editor.axis_secondary.x == 0.0f && \
ctx->editor.axis_secondary.y == 0.0f && \
ctx->editor.axis_secondary.z == 0.0f);
			if (ctx->editor.mode == EDIT_SCALE)
			{
				obj_screen_pos = world_to_screen(ctx, ctx->editor.selected_obj->transform.pos);
				dir_mouse_to_obj_screen.x = (float)ctx->mouse.pos_orig.x - obj_screen_pos.x;
				dir_mouse_to_obj_screen.y = (float)ctx->mouse.pos_orig.y - obj_screen_pos.y;
				dist_mouse_to_obj_screen = sqrtf(dir_mouse_to_obj_screen.x * dir_mouse_to_obj_screen.x + dir_mouse_to_obj_screen.y * dir_mouse_to_obj_screen.y);
				if (dist_mouse_to_obj_screen > 1.0f)
				{
					dir_mouse_to_obj_screen.x /= dist_mouse_to_obj_screen;
					dir_mouse_to_obj_screen.y /= dist_mouse_to_obj_screen;
					magnitude = ((float)delta.x * dir_mouse_to_obj_screen.x + (float)delta.y * dir_mouse_to_obj_screen.y) * speed;
				}
				else
					magnitude = (float)(delta.x - delta.y) * speed;
			}
			else
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
					magnitude = (float)(delta.x - delta.y) * speed;
					axis_delta = vec3_add(\
	vec3_scale(ctx->editor.axis_primary, delta.x * speed), \
	vec3_scale(ctx->editor.axis_secondary, -delta.y * speed));
				}
			}
			if (ctx->editor.mode == EDIT_TRANSLATE)
				obj_translate(ctx->editor.selected_obj, axis_delta);
			else if (ctx->editor.mode == EDIT_ROTATE)
			{
				if (ctx->editor.constraints == AXIS_DEFAULT)
					axis_delta = ctx->scene.cam.forward;
				else if (is_single_constraint)
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
				obj_rotate(ctx->editor.selected_obj, axis_delta, magnitude);
			}
			else if (ctx->editor.mode == EDIT_SCALE)
			{
				if (ctx->editor.constraints == AXIS_DEFAULT)
					axis_delta = vec3_n(expf(magnitude));
				else
				{
					m = &ctx->editor.selected_obj->transform.object_to_world;
					local_x = vec3_normalize(vec3(m->m[0][0], m->m[1][0], m->m[2][0]));
					local_y = vec3_normalize(vec3(m->m[0][1], m->m[1][1], m->m[2][1]));
					local_z = vec3_normalize(vec3(m->m[0][2], m->m[1][2], m->m[2][2]));

					w_x.u = vec3_dot(ctx->editor.axis_primary, local_x);
					w_x.v = vec3_dot(ctx->editor.axis_secondary, local_x);
					axis_delta.x = expf(sqrtf(w_x.u * w_x.u + w_x.v * w_x.v) * magnitude);

					w_y.u = vec3_dot(ctx->editor.axis_primary, local_y);
					w_y.v = vec3_dot(ctx->editor.axis_secondary, local_y);
					axis_delta.y = expf(sqrtf(w_y.u * w_y.u + w_y.v * w_y.v) * magnitude);

					w_z.u = vec3_dot(ctx->editor.axis_primary, local_z);
					w_z.v = vec3_dot(ctx->editor.axis_secondary, local_z);
					axis_delta.z = expf(sqrtf(w_z.u * w_z.u + w_z.v * w_z.v) * magnitude);
				}
				obj_scale(ctx->editor.selected_obj, axis_delta);
			}
			update_transform(&ctx->editor.selected_obj->transform);
			uint32_t i = 0;
			t_light	*light;
			float	max_scale;
			while (i < ctx->scene.env.lights.total)
			{
				light = ((t_light **)ctx->scene.env.lights.items)[i++];
				max_scale = fminf(fminf(light->obj->transform.scale.x, light->obj->transform.scale.y), light->obj->transform.scale.z);
				light->radius = light->obj->shape.sphere.radius * max_scale;
				light->radius_sq = light->radius * light->radius;
			}
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
	ctx->editor.constraints = AXIS_DEFAULT;
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
