#include "lights.h"
#include "rendering.h"
#include "scene.h"

bool	in_shadow(const t_scene *scene, const t_hit *surf, t_light *light)
{
	t_ray	ray;
	t_hit	hit;

	ray = new_ray(surf->point, light->transform.pos);
	hit = (t_hit){0};
	hit.t = vec3_length(vec3_sub(light->transform.pos, surf->point));
	if (hit_bvh(scene->bvh_root, &ray, &hit))
		return (true);
	return (false);
}
