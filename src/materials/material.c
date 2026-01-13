#include "materials.h"
#include "utils.h"

uint32_t	new_material(t_context *ctx, t_material *mat)
{
	t_material		*new_mat;

	new_mat = malloc(sizeof(*new_mat));
	if (!new_mat)
		fatal_error(ctx, errors(ERR_MATADD), __FILE__, __LINE__);
	// mat->albedo = vec3_n(1.0f);
	// mat->emission = vec3_n(0.0f);
	*new_mat = *mat;
	vector_try_add(ctx, &ctx->scene.materials, new_mat);
	return (ctx->scene.materials.total - 1);
}
