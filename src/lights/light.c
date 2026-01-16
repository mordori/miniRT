#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "libft_random.h"

static inline t_vec3	add_light(const t_vec3 emission, const t_vec3 albedo, float ndotl, float dist);

t_vec3	compute_lighting(const t_scene *scene, const t_hit *hit, t_material *mat, uint32_t *seed)
{
	t_light		*light;
	t_vec3		color;
	t_vec3		hit_to_light;
	t_vec3		orig_biased;
	t_vec3		pos;
	t_vec3		dir;
	float		dist;
	float		ndotl;
	float		cos_light;

	color = (t_vec3){0};
	light = ((t_light **)scene->lights.items)[random_uint32(seed) % scene->lights.total];
	if (light->type == LIGHT_AREA)
		pos = random_point_on_object(light->obj, seed);
	else
		pos = light->pos;
	orig_biased = vec3_add(hit->point, vec3_scale(hit->normal, SHADOW_BIAS));
	hit_to_light = vec3_sub(pos, orig_biased);
	dist = vec3_length(hit_to_light);
	if (dist < LEN_EPSILON)
		return (color);
	dir = vec3_scale(hit_to_light, 1.0f / dist);
	ndotl = vec3_dot(hit->normal, dir);
	if (ndotl <= 0.0f)
		return (color);
	if (light->type == LIGHT_AREA)
	{
		cos_light = vec3_dot(normal_at_point(light->obj, pos), vec3_scale(dir, -1.0f));
		if (cos_light <= 0.0f)
			return (color);
		ndotl *= cos_light;
	}
	if (hit_shadow(scene, orig_biased, dir, dist - 1e-3f))
		return (color);
	color = vec3_scale(add_light(light->emission, mat->albedo, ndotl, dist), (float)scene->lights.total);
	if (light->type == LIGHT_AREA)
	{
		float area = 4.0f * M_PI * light->obj->shape.sphere.radius_squared;
		color = vec3_scale(color, area);
	}
	return (color);
}

t_vec3	compute_ambient(const t_scene *scene, t_material *mat)
{
	t_light		*light;
	t_vec3		color;

	light = (t_light *)&scene->ambient_light;
	color = vec3_mul(light->color, mat->albedo);
	color = vec3_scale(color, light->intensity);
	return (color);
}

t_vec3	compute_directional(const t_scene *scene, const t_hit *hit, t_material *mat)
{
	t_light		*light;
	t_vec3		color;
	t_vec3		dir;
	float		ndotl;
	t_vec3		orig_biased;

	light = (t_light *)&scene->directional_light;
	orig_biased = vec3_add(hit->point, vec3_scale(hit->normal, SHADOW_BIAS));
	dir = light->dir;
	ndotl = vec3_dot(hit->normal, dir);
	if (ndotl <= 0.0f || hit_shadow(scene, orig_biased, dir, M_INF))
		return ((t_vec3){0});
	color = vec3_mul(light->color, mat->albedo);
	color = vec3_scale(color, light->intensity * ndotl);
	return (color);
}

static inline t_vec3	add_light(const t_vec3 emission, const t_vec3 albedo, float ndotl, float dist)
{
	t_vec3		result;
	float		attenuation;

	attenuation = ndotl / fmaxf(dist * dist, 0.1f);
	result = vec3_mul(emission, albedo);
	result = vec3_scale(result, attenuation);
	return (result);
}
