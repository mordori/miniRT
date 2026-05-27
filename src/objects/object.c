#include "defines.h"
#include "objects.h"
#include "parsing.h"
#include "utils.h"

t_error add_object(t_context* ctx, t_object* obj) {
	t_object* new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return E_MALLOC;

	obj->transform.scale = g_one;
	update_transform(&obj->transform);
	update_bounds(obj);
	if (get_max_bounds_dim(obj) > WORLD_LIMIT) {
		free(new_obj);
		return E_TOO_BIG;
	}

	*new_obj = *obj;
	new_obj->mat = ((t_material**)ctx->scene.assets.materials.items)[obj->material_id];
	new_obj->flags = new_obj->mat->flags;
	if (!vector_add(&ctx->scene.geo.objs, new_obj)) {
		free(new_obj);
		return E_MALLOC;
	}
	return E_OK;
}

bool hit_object(const t_object* obj, const t_ray* ray, t_hit* hit) {
	if (!obj)
		return false;
	t_ray r = ray_world_to_object(ray, &obj->transform.world_to_object);

	bool result = false;
	switch (obj->type) {
		case OBJ_SPHERE: result = hit_sphere(&obj->shape, &r, hit); break;
		case OBJ_CYLINDER: result = hit_cylinder(&obj->shape, &r, hit); break;
		case OBJ_CONE: result = hit_cone(&obj->shape, &r, hit); break;
		case OBJ_QUAD: result = hit_quad(&obj->shape, &r, hit); break;
		case OBJ_PLANE: result = hit_plane(&obj->shape, &r, hit); break;
	}
	if (result) {
		hit->obj = (t_object*)obj;
		hit_object_to_world(hit, &obj->transform);
	}
	return result;
}
