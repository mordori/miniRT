#include <stdbool.h>

#include "defines.h"
#include "editing.h"
#include "lib_math.h"
#include "objects.h"
#include "parsing.h"
#include "scene.h"
#include "utils.h"

t_error add_object(t_context* ctx, t_object* obj, bool is_selected) {
	t_object* new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return E_MALLOC;

	obj->transform.scale = g_one;
	obj->transform.rot_euler = quat_to_euler(obj->transform.rot);
	update_transform(&obj->transform);
	update_bounds(obj);
	if (get_max_bounds_dim(obj) > WORLD_LIMIT) {
		free(new_obj);
		return E_TOO_BIG;
	}

	*new_obj = *obj;
	new_obj->mat = ((t_material**)ctx->scene.assets.materials.items)[obj->material_id];
	new_obj->flags = new_obj->mat->flags;
	if (is_selected) {
		ctx->editor.selected_obj = new_obj;
	} else if (!vector_add(&ctx->scene.geo.objs, new_obj)) {
		free(new_obj);
		return E_MALLOC;
	}
	return E_OK;
}

bool del_object(t_context* ctx) {
	t_object* obj = ctx->editor.selected_obj;
	if (!obj)
		return false;

	t_renderer* r = &ctx->renderer;

	ctx->editor.selected_obj = NULL;
	for (uint32_t i = 0; i < ctx->scene.env.lights.total; ++i) {
		t_light* light = ctx->scene.env.lights.items[i];
		if (light->obj == obj) {
			vector_del2(&ctx->scene.env.lights, light);
			for (uint32_t j = 0; j < ctx->scene.env.lights.total; ++j)
				((t_light**)ctx->scene.env.lights.items)[j]->idx = j;
			ctx->bn_stride = (BN_CO_U + ((ctx->scene.env.lights.total + 1) * 2) + 3) & ~3;
			break;
		}
	}
	vector_del2(&ctx->scene.geo.objs, obj);

	if (!init_bvh(ctx)) {
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	}

	return true;
}

bool dup_object(t_context* ctx) {
	t_object* obj = ctx->editor.selected_obj;
	if (!obj)
		return false;

	t_renderer* r = &ctx->renderer;
	atomic_store(&r->render_cancel, true);
	while (r->threads_running)
		pthread_cond_wait(&r->cond, &r->mutex);

	deselect_object(ctx);
	if (add_object(ctx, obj, true) == E_OK) {
		ctx->editor.orig_transform = ctx->editor.selected_obj->transform;
		begin_edit_action(ctx, EDIT_TRANSLATE);
		ctx->editor.mode = EDIT_TRANSLATE;
		return true;
	} else {
		return false;
	}
}

bool hit_object(const t_object* obj, const t_ray* ray, t_hit* hit) {
	if (!obj)
		return false;

	const t_shape* shape = &obj->shape;
	t_ray r = ray_world_to_object(ray, &obj->transform.world_to_object);
	bool result = false;
	switch (obj->type) {
		case OBJ_SPHERE: result = hit_sphere(shape, &r, hit); break;
		case OBJ_CYLINDER: result = hit_cylinder(shape, &r, hit); break;
		case OBJ_CONE: result = hit_cone(shape, &r, hit); break;
		case OBJ_QUAD: result = hit_quad(shape, &r, hit, obj->flags); break;
		case OBJ_PLANE: result = hit_plane(shape, &r, hit, obj->flags); break;
		case OBJ_MESH: result = hit_mesh(shape, &r, hit, obj->flags); break;
	}
	if (result) {
		hit->obj = (t_object*)obj;
		hit_object_to_world(hit, &obj->transform);
	}
	return result;
}

void update_transform(t_transform* t) {
	t_mat4 translation = mat4_translate(t->pos);
	t_mat4 rotation = quat_to_mat4(t->rot);
	t_mat4 scale = mat4_scale(t->scale);

	t->object_to_world = mat4_mul(&translation, &rotation);
	t->object_to_world = mat4_mul(&t->object_to_world, &scale);

	mat4_inverse(&t->object_to_world, &t->world_to_object);
}
