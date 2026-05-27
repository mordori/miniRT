#include "scene.h"
#include "utils.h"

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
