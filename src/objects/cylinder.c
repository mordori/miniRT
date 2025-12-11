#include "objects.h"

bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_cylinder	cylinder;
	float		t;

	(void)t;
	(void)cylinder;
	(void)ray;
	(void)hit;
	cylinder = shape->cylinder;
	return (true);
}
