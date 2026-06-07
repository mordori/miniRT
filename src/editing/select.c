#include "defines.h"
#include "editing.h"
#include "lib_math.h"
#include "objects.h"
#include "scene.h"
#include "utils.h"

static inline void select_obj(t_context* ctx, t_renderer* r, t_ray* ray, t_hit* hit);

void select_object(t_context* ctx) {
	t_vec2i mouse;
	mlx_get_mouse_pos(ctx->mlx, &mouse.x, &mouse.y);

	t_renderer* r = &ctx->renderer;
	t_vec3 pixel = vec3_add(vec3_scale(r->cam.viewport.d_u, (float)mouse.x + 0.5f), vec3_scale(r->cam.viewport.d_v, (float)mouse.y + 0.5f));
	t_vec3 pixel_loc = vec3_add(r->cam.viewport.pixel_00_loc, pixel);
	t_ray ray = new_ray(r->cam.transform.pos, vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos)));
	t_hit hit = new_hit(0);

	bool hitBVH = hit_bvh_editing(ctx->scene.geo.bvh_root_idx, &ray, &hit, ctx->scene.geo.bvh_nodes);
	bool hitPlanes = hit_planes(ctx, &ray, &hit);
	if (hitBVH || hitPlanes)
		select_obj(ctx, r, &ray, &hit);
	else if (ctx->editor.selected_obj && !hit_object(ctx->editor.selected_obj, &ray, &hit))
		deselect_object(ctx, r);
}

static inline void select_obj(t_context* ctx, t_renderer* r, t_ray* ray, t_hit* hit) {
	bool hitSelectedObj = hit_object(ctx->editor.selected_obj, ray, hit);
	if (hitSelectedObj || hit->obj == ctx->scene.env.dir_light.obj)
		return;

	bool rebuild_bvh = false;
	if (ctx->editor.selected_obj) {
		if (ctx->editor.selected_obj->type == OBJ_PLANE)
			vector_try_add(ctx, &ctx->scene.geo.planes, ctx->editor.selected_obj);
		else
			rebuild_bvh = vector_try_add(ctx, &ctx->scene.geo.objs, ctx->editor.selected_obj);
	}

	if (hit->obj->type == OBJ_PLANE)
		vector_remove(&ctx->scene.geo.planes, hit->obj);
	else
		rebuild_bvh = vector_remove(&ctx->scene.geo.objs, hit->obj);
	ctx->editor.selected_obj = hit->obj;
	ctx->scene.cam.distance = fmaxf(vec3_dist(ctx->scene.cam.transform.pos, hit->obj->transform.pos), 0.01f);

	if (rebuild_bvh && !init_bvh(ctx)) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	}
	ctx->editor.request_ui_tab = true;
}

bool deselect_object(t_context* ctx, t_renderer* r) {
	t_object* obj = ctx->editor.selected_obj;
	if (!obj)
		return false;

	if (ctx->editor.mode != EDIT_DEFAULT)
		cancel_edit_action(ctx);
	ctx->editor.selected_obj = NULL;

	if (obj->type == OBJ_PLANE) {
		vector_try_add(ctx, &ctx->scene.geo.planes, obj);
		return true;
	}

	vector_try_add(ctx, &ctx->scene.geo.objs, obj);
	if (!init_bvh(ctx)) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	}
	return true;
}
