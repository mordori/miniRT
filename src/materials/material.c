#include "materials.h"
#include "utils.h"

void	new_material(t_context *ctx)
{
	t_material		*mat;
	static int		i = 0;

	mat = malloc(sizeof(*mat));
	if (!mat)
		fatal_error(ctx, errors(ERR_MATADD), __FILE__, __LINE__);
	vector_try_add(ctx, &ctx->scene.materials, mat);

	// For testing rendering
	// -----------------------
		mat->color = vec4_n(1.0f);
		mat->color = (t_vec4){{1.0f, 1.0f, 1.0f * i, 1.0f}};
	// -----------------------
	++i;
	// set_texture();
}
