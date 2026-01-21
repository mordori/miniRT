#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "libft_random.h"

static inline t_vec3	add_light(const t_vec3 emission, const t_vec3 albedo, float ndotl, float dist);

t_vec3	compute_lighting(const t_context *ctx, const t_path *path, const t_light *light, t_pixel *pixel)
{
	t_vec3		color;
	t_vec3		hit_to_light;
	t_vec3		orig_biased;
	t_vec3		pos;
	t_vec3		dir;
	t_vec2		uv;
	float		dist;
	float		ndotl;
	float		cos_light;
	float		area;

	color = (t_vec3){0};
	if (light->type >= LIGHT_AREA)
	{
		if (path->bounce == 0)
			uv = (t_vec2){{blue_noise(&ctx->blue_noise, pixel, 4), blue_noise(&ctx->blue_noise, pixel, 5)}};
		else
			uv = (t_vec2){{randomf01(pixel->seed), randomf01(pixel->seed)}};
		pos = random_point_on_object(light->obj, uv.u, uv.v);
	}
	else
		pos = light->pos;
	orig_biased = vec3_add(path->hit.point, vec3_scale(path->hit.normal, G_EPSILON));
	hit_to_light = vec3_sub(pos, orig_biased);
	dist = vec3_length(hit_to_light);
	if (dist < LEN_EPSILON)
		return (color);
	dir = vec3_scale(hit_to_light, 1.0f / dist);
	ndotl = vec3_dot(path->hit.normal, dir);
	if (ndotl <= 0.0f)
		return (color);
	if (light->type >= LIGHT_AREA)
	{
		cos_light = vec3_dot(normal_at_point(light->obj, pos), vec3_scale(dir, -1.0f));
		if (cos_light <= 0.0f)
			return (color);
		ndotl *= cos_light;
	}
	if (hit_shadow(&ctx->scene, orig_biased, dir, dist - G_EPSILON))
		return (color);
	color = add_light(light->emission, path->mat->albedo, ndotl, dist);
	if (light->type != LIGHT_DIRECTIONAL)
		color = vec3_scale(color, (float)ctx->scene.lights.total);
	if (light->type >= LIGHT_AREA)
	{
		area = 4.0f * M_PI * light->obj->shape.sphere.radius_squared;
		color = vec3_scale(color, area);
	}
	return (color);
}

t_vec3	compute_ambient(const t_scene *scene, const t_material *mat)
{
	t_light		*light;
	t_vec3		color;

	light = (t_light *)&scene->ambient_light;
	color = vec3_mul(light->color, mat->albedo);
	color = vec3_scale(color, light->intensity);
	return (color);
}

t_vec3	compute_directional(const t_scene *scene, const t_hit *hit, const t_material *mat)
{
	t_light		*light;
	t_vec3		color;
	t_vec3		dir;
	float		ndotl;
	t_vec3		orig_biased;

	light = (t_light *)&scene->directional_light;
	orig_biased = vec3_add(hit->point, vec3_scale(hit->normal, G_EPSILON));
	dir = vec3_normalize(light->dir);
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
