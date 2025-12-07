#include "utils.h"

t_vec3	point(t_ray ray, float t)
{
	return (vec3_add(ray.origin, vec3_scale(ray.dir, t)));
}
