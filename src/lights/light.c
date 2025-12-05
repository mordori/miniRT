#include "lights.h"
#include "utils.h"

static inline t_light	init_light(char **params);

void	add_light(t_context *ctx, char **params)
{
	t_light	light;

	light = init_light(params);
	(void)light;
	(void)ctx;
	// if (*params == LIGHT_AMBIENT)
	// 	init_ambient_light();
	// else if (*params == LIGHT_DIRECTIONAL)
	// 	init_directional_light();
	// else if (*params == LIGHT_POINT)
	// 	init_point_light();
}

static inline t_light	init_light(char **params)
{
	t_light	light;

	(void)params;
	// light.type = :
	// light.transform.pos = ;
	// light.intensity = ;
	light.color = (t_color){(255 << 24) | (255 << 16) | (255 << 8) | 0xFF}; // {R | G | B | A} 32bit uint combined
	return (light);
}
