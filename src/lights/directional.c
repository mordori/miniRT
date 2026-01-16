#include "lights.h"

void	init_directional_light(t_light *light)
{
	light->dir = (t_vec3){{20.0f, 13.75f, 14.75f}};
	light->dir = vec3_normalize(light->dir);
	light->color = vec3_n(1.0f);
	light->intensity = 1.0f;
	light->type = LIGHT_DIRECTIONAL;
}
