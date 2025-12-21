#include "lights.h"
#include "utils.h"

static inline t_light	init_light(char **params);
static inline t_vec3	calculate_light(const t_light *light, t_vec4 color, float ndotl, float dist);

void	add_light(t_context *ctx, char **params)
{
	t_light		light;

	light = init_light(params);
	// if (light.type == LIGHT_AMBIENT)
	// 	init_ambient_light();
	// else if (light.type  == LIGHT_DIRECTIONAL)
	// 	init_directional_light();
	// else if (light.type  == LIGHT_POINT)
		init_point_light(ctx, &light);
}

static inline t_light	init_light(char **params)
{
	t_light		light;

	(void)params;
	light = (t_light){0};

	// For testing rendering
	// -----------------------
		light.type = LIGHT_POINT;
		light.intensity = 100.0f;
		light.transform.pos = (t_vec3){{2.0f, 0.0f, 5.0f}};
		light.color = (t_vec4){{0.0f, 0.2f, 6.5f, 1.0f}};
	// -----------------------

	return (light);
}

t_vec4	calculate_lighting(const t_scene *scene, const t_hit *hit, size_t idx, float ndotl)
{
	t_light		*light;
	t_vec4		color;
	t_light		**lights;
	t_vec3		dir;
	float		dist;

	color = (t_vec4){0};
	color.rgb = vec3_scale(vec3_mul(scene->ambient_light.color.rgb, hit->color.rgb), scene->ambient_light.intensity);
	lights = (t_light **)scene->lights.items;
	idx = 0;
	while (idx < scene->lights.total)
	{
		light = lights[idx++];
		dir = vec3_sub(light->transform.pos, hit->point);
		dist = vec3_length(dir);
		if (dist < 1e-6f)
			continue ;
		ndotl = vec3_dot(hit->normal, vec3_scale(dir, 1.0f / dist));
		if (ndotl <= 0.0f || hit_shadow(scene, hit, light, dist))
			continue ;
		color.rgb = vec3_add(color.rgb, calculate_light(light, hit->color, ndotl, dist));
	}
	return (color);
}

static inline t_vec3	calculate_light(const t_light *light, t_vec4 color, float ndotl, float dist)
{
	t_vec4		result;
	float		factor;

	// Bonus
	factor = (light->intensity * ndotl) / (dist * dist);

	// Mandatory
	// factor = (light->intensity * ndotl / (1.0f + (0.9f * dist) + (0.032f * dist * dist)));

	result.rgb = vec3_scale(vec3_mul(light->color.rgb, color.rgb), factor);
	return (result.rgb);
}
