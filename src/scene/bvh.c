#include "objects.h"
#include "scene.h"
#include "utils.h"

static inline uint32_t build_bvh(t_context* ctx, const t_object** objs, size_t n, uint32_t* nodes);
static inline void sort_bvh_objects(t_bvh_node* node, const t_object** objs, size_t n);
static inline void branch_idx(const t_ray* ray, const t_bvh_node* node, uint32_t* stack, int32_t* i);

bool init_bvh(t_context* ctx) {
	t_scene* scene = &ctx->scene;
	free(scene->geo.bvh_nodes);
	scene->geo.bvh_nodes = NULL;

	size_t n = scene->geo.objs.total;
	if (n == 0) {
		scene->geo.bvh_root_idx = 0;
		return true;
	}

	scene->geo.bvh_nodes = malloc(sizeof(t_bvh_node) * ((2 * n) - 1));
	if (!scene->geo.bvh_nodes)
		return false;

	t_object** objs = (t_object**)scene->geo.objs.items;
	uint32_t nodes = 1;
	scene->geo.bvh_root_idx = build_bvh(ctx, (const t_object**)objs, n, &nodes);
	return true;
}

static inline uint32_t build_bvh(t_context* ctx, const t_object** objs, size_t n, uint32_t* nodes) {
	int32_t idx = (*nodes)++;
	t_bvh_node* node = &ctx->scene.geo.bvh_nodes[idx - 1];
	memset(node, 0, sizeof(t_bvh_node));

	if (n == 1) {
		node->obj = (t_object*)objs[0];
		node->aabb = get_object_bounds(node->obj);
		return idx;
	}

	node->aabb = get_volume_bounds((t_object**)objs, n);
	sort_bvh_objects(node, objs, n);

	size_t half_n = n / 2;
	node->left_idx = build_bvh(ctx, objs, half_n, nodes);
	node->right_idx = build_bvh(ctx, objs + half_n, n - half_n, nodes);
	return idx;
}

static inline void sort_bvh_objects(t_bvh_node* node, const t_object** objs, size_t n) {
	t_vec3 size = vec3_sub(node->aabb.max, node->aabb.min);

	node->axis = 0;
	if (size.y > size.x && size.y > size.z)
		node->axis = 1;
	else if (size.z > size.x && size.z > size.y)
		node->axis = 2;

	switch (node->axis) {
		case 0: qsort(objs, n, sizeof(t_object*), cmp_bounds_x); break;
		case 1: qsort(objs, n, sizeof(t_object*), cmp_bounds_y); break;
		case 2: qsort(objs, n, sizeof(t_object*), cmp_bounds_z); break;
	}
}

bool hit_bvh(uint32_t root_idx, const t_ray* ray, t_hit* hit, t_bvh_node* tree) {
	if (!root_idx)
		return false;

	t_bvh_element bvh = { 0 };
	bvh.stack[bvh.i++] = root_idx;

	while (bvh.i > 0) {
		bvh.node = &tree[bvh.stack[--bvh.i] - 1];
		if (!hit_aabb(&bvh.node->aabb, ray, hit->t))
			continue;
		if (bvh.node->obj) {
			bvh.temp = *hit;
			if (!(bvh.node->obj->flags & OBJ_HIDDEN_SCENE) && !(hit->is_primary && (bvh.node->obj->flags & OBJ_HIDDEN_CAM)) &&
				hit_object(bvh.node->obj, ray, &bvh.temp)) {
				bvh.res = true;
				*hit = bvh.temp;
			}
			continue;
		}
		branch_idx(ray, bvh.node, bvh.stack, &bvh.i);
	}
	return bvh.res;
}

bool hit_bvh_shadow(uint32_t root_idx, const t_ray* ray, float dist, t_bvh_node* tree) {
	if (!root_idx)
		return false;

	int32_t i = 0;
	uint32_t stack[64];
	stack[i++] = root_idx;

	while (i > 0) {
		t_bvh_node* node = &tree[stack[--i] - 1];
		if (!hit_aabb(&node->aabb, ray, dist))
			continue;
		if (node->obj) {
			t_hit temp = { .t = dist };
			if (!(node->obj->flags & OBJ_NO_CAST_SHADOW) && hit_object(node->obj, ray, &temp))
				return true;
			continue;
		}
		branch_idx(ray, node, stack, &i);
	}
	return false;
}

bool hit_bvh_editing(uint32_t root_idx, const t_ray* ray, t_hit* hit, t_bvh_node* tree) {
	if (!root_idx)
		return false;

	t_bvh_element bvh = { 0 };
	bvh.stack[bvh.i++] = root_idx;

	while (bvh.i > 0) {
		bvh.node = &tree[bvh.stack[--bvh.i] - 1];
		if (!hit_aabb(&bvh.node->aabb, ray, hit->t))
			continue;
		if (bvh.node->obj) {
			bvh.temp = *hit;
			if (!(bvh.node->obj->flags & OBJ_HIDDEN_SCENE) && hit_object(bvh.node->obj, ray, &bvh.temp)) {
				bvh.res = true;
				*hit = bvh.temp;
			}
			continue;
		}
		branch_idx(ray, bvh.node, bvh.stack, &bvh.i);
	}
	return bvh.res;
}

static inline void branch_idx(const t_ray* ray, const t_bvh_node* node, uint32_t* stack, int32_t* i) {
	uint32_t mask = 0u - (uint32_t)(ray->signs[node->axis]);
	stack[(*i)++] = (node->left_idx & mask) | (node->right_idx & ~mask);
	stack[(*i)++] = (node->right_idx & mask) | (node->left_idx & ~mask);
}
