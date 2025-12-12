#include "materials.h"

t_material	set_material(char **params)
{
	t_material	mat;

	// For testing rendering
	// -----------------------
		(void)params;
		mat.color = vec4_n(1.0f);
	// -----------------------

	// set_texture();
	return (mat);
}
