#include "utils.h"

t_vec3	get_point(t_ray *ray, float t)
{
	return (vec3_add(ray->origin, vec3_scale(ray->dir, t)));
}
