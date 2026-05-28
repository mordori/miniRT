#include "editing.h"
#include "lib_math.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"
#include "utils.h"

static inline float get_local_axis_component(t_context* ctx, float magnitude, const t_mat4* m, uint32_t i);

void obj_scale(t_context* ctx, float magnitude) {
	t_object* obj = ctx->editor.selected_obj;
	t_vec3 prev_scale = obj->transform.scale;

	t_vec3 axis;
	if (ctx->editor.constraint_axis == AXIS_DEFAULT) {
		axis = vec3_n(expf(magnitude));
	} else {
		t_mat4* m = &obj->transform.object_to_world;
		axis = (t_vec3){ //
			.x = get_local_axis_component(ctx, magnitude, m, 0),
			.y = get_local_axis_component(ctx, magnitude, m, 1),
			.z = get_local_axis_component(ctx, magnitude, m, 2)
		};
	}
	t_vec3 scale = vec3_mul(obj->transform.scale, axis);
	obj->transform.scale.v = _mm_max_ps(scale.v, (t_vec4){ { 0.1f, 0.1f, 0.1f, 1.0f } }.v);
	update_transform(&obj->transform);
	update_bounds(obj);
	if (get_max_bounds_dim(obj) > WORLD_LIMIT)
		obj->transform.scale = prev_scale;
}

float eval_scale_magnitude(t_context* ctx, t_vec2i delta, float speed) {
	static t_vec2 v_mouse;
	static t_vec2i orig;

	if (orig.x != ctx->mouse.pos_orig.x || orig.y != ctx->mouse.pos_orig.y) {
		orig = ctx->mouse.pos_orig;
		v_mouse = vec2((float)orig.x, (float)orig.y);
	}

	t_vec2 obj_pos_SS = world_to_screen(ctx, &ctx->scene.cam, &ctx->scene.cam.viewport, ctx->editor.selected_obj->transform.pos);
	float dist_prev = vec3_dist(vec3(v_mouse.x, v_mouse.y, 0.0f), vec3(obj_pos_SS.x, obj_pos_SS.y, 0.0f));
	v_mouse.x += (float)delta.x;
	v_mouse.y += (float)delta.y;
	float dist_curr = vec3_dist(vec3(v_mouse.x, v_mouse.y, 0.0f), vec3(obj_pos_SS.x, obj_pos_SS.y, 0.0f));
	return (dist_curr - dist_prev) * speed;
}

static inline float get_local_axis_component(t_context* ctx, float magnitude, const t_mat4* m, uint32_t i) {
	t_vec3 local = vec3_normalize(vec3(m->m[0][i], m->m[1][i], m->m[2][i]));
	t_vec2 w = { //
		.u = vec3_dot(ctx->editor.axis_primary, local),
		.v = vec3_dot(ctx->editor.axis_secondary, local)
	};
	return expf(sqrtf(w.u * w.u + w.v * w.v) * magnitude);
}
