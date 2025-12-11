#include "materials.h"

t_material	set_material(char **params)
{
	t_material	mat;

	// For testing rendering
	// -----------------------
		(void)params;
		mat.color = (t_vec4){{0.0f, 0.0f, 1.0f, 1.0f}};
	// -----------------------

	// set_texture();
	return (mat);
}
