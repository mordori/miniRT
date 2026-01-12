#include "lights.h"
#include "utils.h"

void	init_point_light(t_context *ctx, t_light *light)
{
	t_light	*l;

	l = malloc(sizeof(*l));
	if (!l)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	*l = *light;
	if (l->intensity < 1.01f)
		l->intensity *= 100.0f;
	vector_try_add(ctx, &ctx->scene.lights, l);
}
