#include "lights.h"
#include "utils.h"

static inline void	compute_directional(const t_scene *scene, const t_hit *hit, t_material *mat, t_vec3 *color);
static inline t_vec3	add_light(const t_light *light, t_vec3 color, float ndotl, float dist);

t_vec3	compute_lighting(const t_scene *scene, const t_hit *hit, size_t idx, t_material *mat)
{
	t_light		*light;
	t_vec3		color;
	t_vec3		dir;
	float		dist;
	float		ndotl;

	color = (t_vec3){0};
	compute_directional(scene, hit, mat, &color);
	idx = 0;
	while (idx < scene->lights.total)
	{
		light = ((t_light **)scene->lights.items)[idx++];
		dir = vec3_sub(light->pos, hit->point);
		dist = vec3_length(dir);
		if (dist < M_EPSILON)
			continue ;
		ndotl = vec3_dot(hit->normal, vec3_scale(dir, 1.0f / dist));
		if (ndotl <= 0.0f || hit_shadow(scene, hit, light, dist))
			continue ;
		color = vec3_add(color, add_light(light, mat->albedo, ndotl, dist));
	}
	return (color);
}

void	compute_ambient(const t_scene *scene, t_material *mat, t_vec3 *color)
{
	t_light		*light;
	t_vec3		c;

	light = (t_light *)&scene->ambient_light;
	c = vec3_mul(light->color, mat->albedo);
	c = vec3_scale(c, light->intensity);
	*color = vec3_add(*color, c);
}

static inline void	compute_directional(const t_scene *scene, const t_hit *hit, t_material *mat, t_vec3 *color)
{
	t_light		*light;
	t_vec3		c;
	t_vec3		dir;
	float		ndotl;

	light = (t_light *)&scene->directional_light;
	dir = light->dir;
	ndotl = vec3_dot(hit->normal, dir);
	if (ndotl <= 0.0f || hit_shadow(scene, hit, light, M_INF))
		return ;
	c = vec3_mul(light->color, mat->albedo);
	c = vec3_scale(c, light->intensity * ndotl);
	*color = vec3_add(*color, c);
}

static inline t_vec3	add_light(const t_light *light, t_vec3 color, float ndotl, float dist)
{
	t_vec3		result;
	float		factor;

	factor = (light->intensity * ndotl) / (dist * dist);
	result = vec3_mul(light->color, color);
	result = vec3_scale(result, factor);
	return (result);
}
