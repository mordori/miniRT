#include <string.h>

#include "defines.h"
#include "scene.h"

static inline t_aabb combine_aabb(const t_aabb* a, const t_aabb* b);

t_aabb get_volume_bounds(t_object** objs, size_t n) {
	t_aabb res = get_object_bounds(objs[0]);
	objs[0]->bounds_center = vec3_div(vec3_add(res.min, res.max), 2.0f);
	objs[0]->bounds = vec3_sub(res.max, res.min);
	size_t i = 1;
	while (i < n) {
		t_aabb aabb = get_object_bounds(objs[i]);
		objs[i]->bounds_center = vec3_div(vec3_add(aabb.min, aabb.max), 2.0f);
		objs[i]->bounds = vec3_sub(aabb.max, aabb.min);
		res = combine_aabb(&res, &aabb);
		++i;
	}
	return res;
}

t_aabb get_object_bounds(const t_object* obj) {
	t_aabb res = { 0 };
	switch (obj->type) {
		case OBJ_SPHERE: res = sphere_bounds(obj); break;
		case OBJ_CYLINDER: res = cylinder_bounds(obj); break;
		case OBJ_CONE: res = cone_bounds(obj); break;
		case OBJ_QUAD: res = quad_bounds(obj); break;
		case OBJ_PLANE: break;
	}
	return res;
}

t_aabb aabb_object_to_world(t_aabb aabb, const t_mat4* object_to_world) {
	t_vec3 corners[8];
	corners[0] = vec3(aabb.min.x, aabb.min.y, aabb.min.z);
	corners[1] = vec3(aabb.max.x, aabb.min.y, aabb.min.z);
	corners[2] = vec3(aabb.min.x, aabb.max.y, aabb.min.z);
	corners[3] = vec3(aabb.max.x, aabb.max.y, aabb.min.z);
	corners[4] = vec3(aabb.min.x, aabb.min.y, aabb.max.z);
	corners[5] = vec3(aabb.max.x, aabb.min.y, aabb.max.z);
	corners[6] = vec3(aabb.min.x, aabb.max.y, aabb.max.z);
	corners[7] = vec3(aabb.max.x, aabb.max.y, aabb.max.z);

	t_aabb res = { .min = vec3_n(M_INF), .max = vec3_n(-M_INF) };
	uint32_t i = 0;
	while (i < 8) {
		t_vec4 p = mat4_mul_vec4(object_to_world, vec4_3(corners[i], 1.0f));
		res.min.v = _mm_min_ps(res.min.v, p.v);
		res.max.v = _mm_max_ps(res.max.v, p.v);
		++i;
	}
	return res;
}

static inline t_aabb combine_aabb(const t_aabb* a, const t_aabb* b) {
	return (t_aabb){ //
		.min.v = _mm_min_ps(a->min.v, b->min.v),
		.max.v = _mm_max_ps(a->max.v, b->max.v)
	};
}

bool hit_aabb(const t_aabb* aabb, const t_ray* ray, float closest_t) {
	t_v4sf t1 = (aabb->min.v - ray->origin.v) * ray->dir_recip.v;
	t_v4sf t2 = (aabb->max.v - ray->origin.v) * ray->dir_recip.v;
	t_v4sf near = _mm_min_ps(t1, t2);
	t_v4sf far = _mm_max_ps(t1, t2);
	float enter = fmaxf(fmaxf(near[0], near[1]), fmaxf(near[2], 0.0f));
	float exit = fminf(fminf(far[0], far[1]), fminf(far[2], closest_t));
	return enter < exit;
}
