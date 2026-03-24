#include "editing.h"
#include "utils.h"

static inline float	get_local_axis_component(\
t_context *ctx, float magnitude, const t_mat4 *m, uint32_t i);

void	obj_scale(t_context *ctx, float magnitude)
{
	t_mat4		*m;
	t_vec3		axis;

	if (ctx->editor.constraint_axis == AXIS_DEFAULT)
		axis = vec3_n(expf(magnitude));
	else
	{
		m = &ctx->editor.selected_obj->transform.object_to_world;
		axis.x = get_local_axis_component(ctx, magnitude, m, 0);
		axis.y = get_local_axis_component(ctx, magnitude, m, 1);
		axis.z = get_local_axis_component(ctx, magnitude, m, 2);
	}
	ctx->editor.selected_obj->transform.scale = \
vec3_mul(ctx->editor.selected_obj->transform.scale, axis);
	ctx->editor.selected_obj->transform.scale.v = \
_mm_max_ps(ctx->editor.selected_obj->transform.scale.v, \
vec4_3(vec3_n(0.01f), 1.0f).v);
}

float	eval_scale_magnitude(t_context *ctx, t_vec2i delta, float speed)
{
	t_vec2		obj_screen_pos;
	t_vec2		dir_mouse_to_obj_screen;
	float		dist_mouse_to_obj_screen;
	float		magnitude;

	obj_screen_pos = world_to_screen(ctx, &ctx->scene.cam, \
&ctx->scene.cam.viewport, ctx->editor.selected_obj->transform.pos);
	dir_mouse_to_obj_screen.x = (float)ctx->mouse.pos_orig.x - obj_screen_pos.x;
	dir_mouse_to_obj_screen.y = (float)ctx->mouse.pos_orig.y - obj_screen_pos.y;
	dist_mouse_to_obj_screen = \
sqrtf(dir_mouse_to_obj_screen.x * dir_mouse_to_obj_screen.x + \
dir_mouse_to_obj_screen.y * dir_mouse_to_obj_screen.y);
	if (dist_mouse_to_obj_screen > 1.0f)
	{
		dir_mouse_to_obj_screen.x /= dist_mouse_to_obj_screen;
		dir_mouse_to_obj_screen.y /= dist_mouse_to_obj_screen;
		magnitude = ((float)delta.x * dir_mouse_to_obj_screen.x + \
(float)delta.y * dir_mouse_to_obj_screen.y) * speed;
	}
	else
		magnitude = ((float)(delta.x - delta.y)) * speed;
	return (magnitude);
}

static inline float	get_local_axis_component(\
t_context *ctx, float magnitude, const t_mat4 *m, uint32_t i)
{
	t_vec3		local;
	t_vec2		w;
	float		res;

	local = vec3_normalize(vec3(m->m[0][i], m->m[1][i], m->m[2][i]));
	w.u = vec3_dot(ctx->editor.axis_primary, local);
	w.v = vec3_dot(ctx->editor.axis_secondary, local);
	res = expf(sqrtf(w.u * w.u + w.v * w.v) * magnitude);
	return (res);
}
