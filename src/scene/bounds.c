#include "scene.h"

t_aabb sphere_bounds(const t_object* obj) {
	t_aabb aabb = { //
		.min = vec3_n(-obj->shape.sphere.radius),
		.max = vec3_n(obj->shape.sphere.radius)
	};
	return aabb_object_to_world(aabb, &obj->transform.object_to_world);
}

t_aabb cylinder_bounds(const t_object* obj) {
	t_cylinder cyl = obj->shape.cylinder;
	t_vec3 a2 = { //
		.x = cyl.axis.x * cyl.axis.x,
		.y = cyl.axis.y * cyl.axis.y,
		.z = cyl.axis.z * cyl.axis.z
	};
	t_vec3 half = { //
		.x = cyl.radius * sqrtf(fmaxf(0.0f, 1.0f - a2.x)) + (cyl.height * 0.5f) * fabsf(cyl.axis.x),
		.y = cyl.radius * sqrtf(fmaxf(0.0f, 1.0f - a2.y)) + (cyl.height * 0.5f) * fabsf(cyl.axis.y),
		.z = cyl.radius * sqrtf(fmaxf(0.0f, 1.0f - a2.z)) + (cyl.height * 0.5f) * fabsf(cyl.axis.z)
	};
	t_aabb aabb = { //
		.min = vec3_negate(half),
		.max = half
	};
	return aabb_object_to_world(aabb, &obj->transform.object_to_world);
}

t_aabb cone_bounds(const t_object* obj) {
	t_cone cone = obj->shape.cone;
	t_vec3 a2 = { //
		.x = cone.axis.x * cone.axis.x,
		.y = cone.axis.y * cone.axis.y,
		.z = cone.axis.z * cone.axis.z
	};
	t_vec3 radial = { //
		.x = cone.base_radius * sqrtf(fmaxf(0.0f, 1.0f - a2.x)),
		.y = cone.base_radius * sqrtf(fmaxf(0.0f, 1.0f - a2.y)),
		.z = cone.base_radius * sqrtf(fmaxf(0.0f, 1.0f - a2.z))
	};
	t_vec3 base_center = vec3_scale(cone.axis, cone.height);
	t_aabb aabb = { //
		.min.v = _mm_min_ps(g_zero.v, vec3_sub(base_center, radial).v),
		.max.v = _mm_max_ps(g_zero.v, vec3_add(base_center, radial).v)
	};
	return aabb_object_to_world(aabb, &obj->transform.object_to_world);
}

t_aabb quad_bounds(const t_object* obj) {
	t_quad quad = obj->shape.quad;
	t_vec3 p1 = vec3_add(quad.q, quad.u);
	t_vec3 p2 = vec3_add(quad.q, quad.v);
	t_vec3 p3 = vec3_add(p1, quad.v);
	t_aabb aabb = { //
		.min.v = _mm_min_ps(_mm_min_ps(quad.q.v, p1.v), _mm_min_ps(p2.v, p3.v)),
		.max.v = _mm_max_ps(_mm_max_ps(quad.q.v, p1.v), _mm_max_ps(p2.v, p3.v))
	};
	aabb.min = vec3_sub(aabb.min, vec3_n(B_EPSILON));
	aabb.max = vec3_add(aabb.max, vec3_n(B_EPSILON));
	return aabb_object_to_world(aabb, &obj->transform.object_to_world);
}

int cmp_bounds_x(const void* a, const void* b) {
	t_object* obj_1 = *(t_object**)a;
	t_object* obj_2 = *(t_object**)b;
	int mask_1 = (obj_1->bounds_center.x < obj_2->bounds_center.x);
	int mask_2 = (obj_1->bounds_center.x > obj_2->bounds_center.x);
	return -mask_1 + mask_2;
}

int cmp_bounds_y(const void* a, const void* b) {
	t_object* obj_1 = *(t_object**)a;
	t_object* obj_2 = *(t_object**)b;
	int mask_1 = (obj_1->bounds_center.y < obj_2->bounds_center.y);
	int mask_2 = (obj_1->bounds_center.y > obj_2->bounds_center.y);
	return -mask_1 + mask_2;
}

int cmp_bounds_z(const void* a, const void* b) {
	t_object* obj_1 = *(t_object**)a;
	t_object* obj_2 = *(t_object**)b;
	int mask_1 = (obj_1->bounds_center.z < obj_2->bounds_center.z);
	int mask_2 = (obj_1->bounds_center.z > obj_2->bounds_center.z);
	return -mask_1 + mask_2;
}

void update_bounds(t_object* obj) {
	if (obj->type == OBJ_PLANE) {
		obj->bounds_center = obj->transform.pos;
	} else {
		t_aabb aabb = get_object_bounds(obj);
		obj->bounds_center = vec3_div(vec3_add(aabb.min, aabb.max), 2.0f);
		obj->bounds = vec3_sub(aabb.max, aabb.min);
	}
}

float get_max_bounds_dim(const t_object* obj) {
	return fmaxf(fmaxf(obj->bounds.x, obj->bounds.y), obj->bounds.z);
}
