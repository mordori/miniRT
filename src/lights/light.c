#include "lights.h"
#include "utils.h"

static inline t_light	init_light(char *src);

void	add_light(t_context *ctx, char *src)
{
	t_light	*light;

	light = malloc(sizeof(*light));
	if (!light)
		fatal_error(ctx, "light malloc failed");
	*light = init_light(src);
	vector_try_add(ctx, &ctx->scene.lights, light);
}

static inline t_light	init_light(char *src)
{
	t_light	light;

	// light.type = :
	// light.transform.pos = ;
	// light.intensity = ;
	light.color = (t_color){(255 << 24) | (255 << 16) | (255 << 8) | 0xFF}; // {R | G | B | A} 32bit uint combined
	if (light.type == LIGHT_DIRECTIONAL)
		light.transform.rot = (t_vec3){0.0f, 0.0f, 0.0f};
	return (light);
}
