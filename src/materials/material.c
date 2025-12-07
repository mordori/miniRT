#include "materials.h"

void	set_material(t_object *obj, char **params)
{
	t_material	mat;

	// For testing rendering
	// -----------------------
		(void)params;
		mat.color = (t_color){RED};
		mat.alpha = 1.0f;
	// -----------------------

	// set_texture();
	obj->material = mat;
}
