#include "objects.h"

bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_plane		plane;
	float		t;

	(void)t;
	(void)plane;
	(void)ray;
	(void)hit;
	plane = shape->plane;
	return (true);
}
