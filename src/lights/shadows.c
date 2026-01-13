#include "lights.h"
#include "rendering.h"
#include "scene.h"

bool	hit_shadow(const t_scene *scene, const t_hit *hit, const t_light *light, float dist)
{
	t_ray		ray;
	t_vec3		origin;
	t_vec3		dir;

	if (dist < SHADOW_BIAS)
		return (false);
	origin = vec3_add(hit->point, vec3_scale(hit->normal, SHADOW_BIAS));
	if (light->type == LIGHT_POINT)
		dir = vec3_normalize(vec3_sub(light->pos, origin));
	else
		dir = light->dir;
	ray = new_ray(origin, dir);
	return (hit_bvh_shadow(scene->bvh_root, &ray, dist));
}
