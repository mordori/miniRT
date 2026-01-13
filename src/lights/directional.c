#include "lights.h"

void	init_directional_light(t_light *light)
{
	light->dir = (t_vec3){{20.0f, 15.0f, 15.0f}};
	light->dir = vec3_normalize(light->dir);
	light->color = (t_vec4){{0.95f, 0.9f, 0.6f, 1.0f}};
	light->intensity = 1.0f;
	light->type = LIGHT_DIRECTIONAL;
}
