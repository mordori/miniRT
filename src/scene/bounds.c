#include <stddef.h>

#include "defines.h"
#include "scene.h"

static inline t_aabb combine_aabb(const t_aabb* a, const t_aabb* b);
static inline t_aabb aabb_object_to_world(t_aabb aabb, const t_mat4* object_to_world);

static inline t_aabb sphere_bounds(const t_object* obj);
static inline t_aabb cylinder_bounds(const t_object* obj);
static inline t_aabb cone_bounds(const t_object* obj);
static inline t_aabb quad_bounds(const t_object* obj);

static inline t_aabb triangle_bounds(const t_triangle* tri);

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
		case OBJ_MESH: {
			t_bvh_node root = obj->shape.mesh.bvh_nodes[obj->shape.mesh.bvh_root_idx - 1];
			res = aabb_object_to_world(root.aabb, &obj->transform.object_to_world);
			break;
		}
	}
	return res;
}

static inline t_aabb aabb_object_to_world(t_aabb aabb, const t_mat4* object_to_world) {
	t_vec3 corners[8];
	corners[0] = vec3(aabb.min.x, aabb.min.y, aabb.min.z);
	corners[1] = vec3(aabb.max.x, aabb.min.y, aabb.min.z);
	corners[2] = vec3(aabb.min.x, aabb.max.y, aabb.min.z);
	corners[3] = vec3(aabb.max.x, aabb.max.y, aabb.min.z);
	corners[4] = vec3(aabb.min.x, aabb.min.y, aabb.max.z);
	corners[5] = vec3(aabb.max.x, aabb.min.y, aabb.max.z);
	corners[6] = vec3(aabb.min.x, aabb.max.y, aabb.max.z);
	corners[7] = vec3(aabb.max.x, aabb.max.y, aabb.max.z);

	t_aabb res = { .min = vec3_n(M_INFf), .max = vec3_n(-M_INFf) };
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

static inline t_aabb sphere_bounds(const t_object* obj) {
	t_aabb aabb = { //
		.min = vec3_n(-obj->shape.sphere.radius),
		.max = vec3_n(obj->shape.sphere.radius)
	};
	return aabb_object_to_world(aabb, &obj->transform.object_to_world);
}

static inline t_aabb cylinder_bounds(const t_object* obj) {
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

static inline t_aabb cone_bounds(const t_object* obj) {
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

static inline t_aabb quad_bounds(const t_object* obj) {
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

static inline t_aabb triangle_bounds(const t_triangle* tri) {
	t_aabb aabb;
	aabb.min.v = _mm_min_ps(_mm_min_ps(tri->v0.v, tri->v1.v), tri->v2.v);
	aabb.max.v = _mm_max_ps(_mm_max_ps(tri->v0.v, tri->v1.v), tri->v2.v);
	aabb.min = vec3_sub(aabb.min, vec3_n(B_EPSILON));
	aabb.max = vec3_add(aabb.max, vec3_n(B_EPSILON));
	return aabb;
}

t_aabb get_mesh_volume_bounds(t_triangle* tris, size_t start, size_t end) {
	t_aabb res = triangle_bounds(&tris[start]);
	for (size_t i = start + 1; i < end; ++i) {
		t_aabb aabb = triangle_bounds(&tris[i]);
		res = combine_aabb(&res, &aabb);
	}
	return res;
}

int cmp_tri_x(const void* a, const void* b) {
	t_triangle* a_tri = (t_triangle*)a;
	t_triangle* b_tri = (t_triangle*)b;
	float a_centroid = a_tri->v0.x + a_tri->v1.x + a_tri->v2.x;
	float b_centroid = b_tri->v0.x + b_tri->v1.x + b_tri->v2.x;
	int mask_1 = (a_centroid < b_centroid);
	int mask_2 = (a_centroid > b_centroid);
	return -mask_1 + mask_2;
}

int cmp_tri_y(const void* a, const void* b) {
	t_triangle* a_tri = (t_triangle*)a;
	t_triangle* b_tri = (t_triangle*)b;
	float a_centroid = a_tri->v0.y + a_tri->v1.y + a_tri->v2.y;
	float b_centroid = b_tri->v0.y + b_tri->v1.y + b_tri->v2.y;
	int mask_1 = (a_centroid < b_centroid);
	int mask_2 = (a_centroid > b_centroid);
	return -mask_1 + mask_2;
}

int cmp_tri_z(const void* a, const void* b) {
	t_triangle* a_tri = (t_triangle*)a;
	t_triangle* b_tri = (t_triangle*)b;
	float a_centroid = a_tri->v0.z + a_tri->v1.z + a_tri->v2.z;
	float b_centroid = b_tri->v0.z + b_tri->v1.z + b_tri->v2.z;
	int mask_1 = (a_centroid < b_centroid);
	int mask_2 = (a_centroid > b_centroid);
	return -mask_1 + mask_2;
}
