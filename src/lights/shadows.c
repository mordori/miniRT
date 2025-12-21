#include "lights.h"
#include "rendering.h"
#include "scene.h"

bool	hit_shadow(const t_scene *scene, const t_hit *hit, const t_light *light, float dist)
{
	static const float	bias = 1e-4f;
	t_ray				ray_shadow;
	t_vec3				origin;

	if (dist < bias)
		return (false);
	origin = vec3_add(hit->point, vec3_scale(hit->normal, bias));
	ray_shadow = new_ray(origin, light->transform.pos);
	return (hit_bvh_shadow(scene->bvh_root, &ray_shadow, dist));
}
