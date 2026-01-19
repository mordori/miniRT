#include "objects.h"

bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_cylinder		cylinder;
	float			t;

	(void)t;
	(void)cylinder;
	(void)ray;
	(void)hit;
	cylinder = shape->cylinder;
	return (true);
}

t_vec3	random_point_on_cylinder(const t_shape *shape, float u, float v)
{
	t_vec3	result;

	(void)shape;
	(void)u;
	(void)v;
	result = vec3_n(1.0f);
	return (result);
}

t_vec3	normal_at_cylinder(const t_shape *shape, const t_vec3 pos)
{
	t_vec3	result;

	(void)shape;
	(void)pos;
	result = vec3_n(1.0f);
	return (result);
}
