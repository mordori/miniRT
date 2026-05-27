#include "defines.h"
#include "utils.h"

t_hit new_hit(uint32_t bounce) {
	return (t_hit){ //
		.t = M_INF,
		.is_primary = (bounce == 0)
	};
}

void hit_object_to_world(t_hit* hit, const t_transform* t) {
	t_vec4 p = vec4_3(hit->point, 1.0f);
	hit->point = mat4_mul_vec4(&t->object_to_world, p).xyz;
	t_mat4 m_inv_t = mat4_transpose(&t->world_to_object);
	t_vec4 n = vec4_3(hit->normal, 0.0f);
	hit->normal = vec3_normalize(mat4_mul_vec4(&m_inv_t, n).xyz);
}

void eval_hit_normal(const t_ray* ray, t_hit* hit, t_vec3 n) {
	if (vec3_dot(ray->dir, n) > 0.0f) {
		hit->normal = vec3_negate(n);
		hit->front_face = false;
	} else {
		hit->normal = n;
		hit->front_face = true;
	}
}
