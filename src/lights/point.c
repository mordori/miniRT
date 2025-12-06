#include "lights.h"
#include "utils.h"

void	add_point_light(t_context *ctx, t_light	light)
{
	t_light	*ptr;

	ptr = malloc(sizeof(*ptr));
	if (!ptr)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	*ptr = light;
	// Add specifics
	vector_try_add(ctx, &ctx->scene.lights, ptr);
}
