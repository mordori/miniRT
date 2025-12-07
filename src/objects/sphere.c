#include "objects.h"

t_shape	init_sphere(char **params)
{
	t_sphere	sphere;
	t_shape		shape;

	// For testing rendering
	// -----------------------
		(void)params;
		sphere.radius = 1.0f;
	// -----------------------

	shape.sphere = sphere;
	return (shape);
}

void	intersect_sphere()
{

}
