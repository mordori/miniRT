#include "utils.h"

t_ray	new_ray(t_vec3 origin, t_vec3 dir)
{
	static const uint32_t	sign_bit = 0x80000000u;
	static const t_v4sf		epsilon = {1e-20f, 1e-20f, 1e-20f, 1e-20f};
	static const t_v4sf		one = {1.0f, 1.0f, 1.0f, 1.0f};
	t_v4ui					signed_eps;
	t_ray					ray;

	ray.origin = origin;
	ray.dir = dir;
	signed_eps = (t_v4ui)epsilon | ((t_v4ui)dir.v & sign_bit);
	ray.dir_recip.v = one / (dir.v + (t_v4sf)signed_eps);
	ray.signs = (t_v4ui)ray.dir_recip.v >> 31;
	return (ray);
}

t_vec3	get_point(const t_ray *ray, float t)
{
	return (vec3_add(ray->origin, vec3_scale(ray->dir, t)));
}

t_hit	new_hit(uint32_t bounce)
{
	t_hit	hit;

	hit = (t_hit){0};
	hit.t = M_INF;
	hit.is_primary = (bounce == 0);
	return (hit);
}
