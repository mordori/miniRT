#include "lights.h"
#include "utils.h"

static inline void	compute_ambient(const t_scene *scene, t_material *mat, t_vec4 *color);
static inline void	compute_directional(const t_scene *scene, const t_hit *hit, t_material *mat, t_vec4 *color);
static inline t_vec3	add_light(const t_light *light, t_vec4 color, float ndotl, float dist);

t_vec4	compute_lighting(const t_scene *scene, const t_hit *hit, size_t idx, t_material *mat)
{
	t_light		*light;
	t_vec4		color;
	t_vec3		dir;
	float		dist;
	float		ndotl;

	color = (t_vec4){0};
	compute_ambient(scene, mat, &color);
	compute_directional(scene, hit, mat, &color);
	idx = 0;
	while (idx < scene->lights.total)
	{
		light = ((t_light **)scene->lights.items)[idx++];
		dir = vec3_sub(light->pos_dir, hit->point);
		dist = vec3_length(dir);
		if (dist < M_EPSILON)
			continue ;
		ndotl = vec3_dot(hit->normal, vec3_scale(dir, 1.0f / dist));
		if (ndotl <= 0.0f || hit_shadow(scene, hit, light, dist))
			continue ;
		color.rgb = vec3_add(color.rgb, add_light(light, mat->color, ndotl, dist));
	}
	return (color);
}

static inline void	compute_ambient(const t_scene *scene, t_material *mat, t_vec4 *color)
{
	t_light		*light;
	t_vec4		c;

	light = (t_light *)&scene->ambient_light;
	c.rgb = vec3_mul(light->color.rgb, mat->color.rgb);
	c.rgb = vec3_scale(c.rgb, light->intensity);
	color->rgb = vec3_add(color->rgb, c.rgb);
}

static inline void	compute_directional(const t_scene *scene, const t_hit *hit, t_material *mat, t_vec4 *color)
{
	t_light		*light;
	t_vec4		c;
	t_vec3		dir;
	float		ndotl;

	light = (t_light *)&scene->directional_light;
	dir = light->pos_dir;
	ndotl = vec3_dot(hit->normal, dir);
	if (ndotl <= 0.0f || hit_shadow(scene, hit, light, M_INF))
		return ;
	c.rgb = vec3_mul(light->color.rgb, mat->color.rgb);
	c.rgb = vec3_scale(c.rgb, light->intensity * ndotl);
	color->rgb = vec3_add(color->rgb, c.rgb);
}

static inline t_vec3	add_light(const t_light *light, t_vec4 color, float ndotl, float dist)
{
	t_vec4		result;
	float		factor;

	factor = (light->intensity * ndotl) / (dist * dist);
	result.rgb = vec3_mul(light->color.rgb, color.rgb);
	result.rgb = vec3_scale(result.rgb, factor);
	return (result.rgb);
}
