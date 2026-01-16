#include "lights.h"
#include "rendering.h"
#include "scene.h"

bool	hit_shadow(const t_scene *scene, const t_vec3 orig, const t_vec3 dir, float dist)
{
	t_ray		ray;

	ray = new_ray(orig, dir);
	return (hit_bvh_shadow(scene->bvh_root, &ray, dist));
}
