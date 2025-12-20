#include "lights.h"
#include "rendering.h"
#include "scene.h"

bool	hit_shadow(const t_scene *scene, const t_hit *hit, t_light *light)
{
	t_ray	ray_shadow;
	t_vec3	origin;
	float	dist;

	origin = vec3_add(hit->point, vec3_scale(hit->normal, 1e-4f));
	ray_shadow = new_ray(origin, light->transform.pos);
	dist = vec3_length(vec3_sub(light->transform.pos, origin));
	return (hit_bvh_shadow(scene->bvh_root, &ray_shadow, dist));
}
