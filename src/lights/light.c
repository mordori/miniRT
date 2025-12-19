#include "lights.h"
#include "utils.h"

static inline t_light	init_light(char **params);

void	add_light(t_context *ctx, char **params)
{
	t_light	light;

	light = init_light(params);
	// if (light.type == LIGHT_AMBIENT)
	// 	init_ambient_light();
	// else if (light.type  == LIGHT_DIRECTIONAL)
	// 	init_directional_light();
	// else if (light.type  == LIGHT_POINT)
		init_point_light(ctx, light);
}

static inline t_light	init_light(char **params)
{
	t_light	light;

	(void)params;
	light = (t_light){0};

	// For testing rendering
	// -----------------------
		light.type = LIGHT_POINT;
		light.intensity = 1.4f;
		light.transform.pos = (t_vec3){{4.5f, 0.0f, 5.0f}};
		light.color = (t_vec4){{0.0f, 0.2f, 6.5f, 1.0f}};
	// -----------------------

	return (light);
}

t_vec4	calculate_lighting(const t_scene *scene, const t_hit *hit)
{
	t_light		*light;
	t_vec4		color;
	t_light		**lights;
	float		ndotl;
	uint32_t	i;
	// float		attenuation;

	color = (t_vec4){0};
	lights = (t_light **)scene->lights.items;
	i = 0;
	while (i < scene->lights.total)
	{
		light = lights[i++];
		ndotl = vec3_dot(hit->normal, vec3_normalize(vec3_sub(light->transform.pos, hit->point)));
		if (ndotl <= 0.0f || in_shadow(scene, hit, light))
			continue ;
		// attenuation + beers law
		color.rgb = vec3_add(color.rgb, vec3_scale(vec3_mul(light->color.rgb, hit->color.rgb), light->intensity * ndotl));
	}
	color.rgb = vec3_add(color.rgb, vec3_scale(vec3_mul(scene->ambient_light.color.rgb, hit->color.rgb), scene->ambient_light.intensity));
	return (color);
}
