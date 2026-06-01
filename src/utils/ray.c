#include "lib_math.h"
#include "utils.h"

t_ray new_ray(t_vec3 origin, t_vec3 dir) {
	static const uint32_t sign_bit = 0x80000000u;
	static const t_v4sf epsilon = { 1e-20f, 1e-20f, 1e-20f, 1e-20f };
	static const t_v4sf one = { 1.0f, 1.0f, 1.0f, 1.0f };

	t_v4ui signed_eps = (t_v4ui)epsilon | ((t_v4ui)dir.v & sign_bit);
	t_v4sf dir_recip = one / (dir.v + (t_v4sf)signed_eps);

	return (t_ray){ //
		.origin = origin,
		.dir = dir,
		.dir_recip.v = dir_recip,
		.signs = (t_v4ui)dir_recip >> 31
	};
}

t_ray ray_world_to_object(const t_ray* ray, const t_mat4* world_to_object) {
	t_vec4 orig = vec4_3(ray->origin, 1.0f);
	t_vec4 dir = vec4_3(ray->dir, 0.0f);

	t_vec3 origOS = mat4_mul_vec4(world_to_object, orig).xyz;
	t_vec3 dirOS = mat4_mul_vec4(world_to_object, dir).xyz;
	return new_ray(origOS, dirOS);
}

t_vec3 get_point(const t_ray* ray, float t) {
	return vec3_add(ray->origin, vec3_scale(ray->dir, t));
}
