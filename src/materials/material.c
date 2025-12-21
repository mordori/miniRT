#include "materials.h"

t_material	set_material(char **params)
{
	t_material		mat;
	static int		i = 0;

	// For testing rendering
	// -----------------------
		(void)params;
		mat.color = vec4_n(1.0f);
		mat.color = (t_vec4){{1.0f, 1.0f, 1.0f * i, 1.0f}};
	// -----------------------
	++i;
	// set_texture();
	return (mat);
}
