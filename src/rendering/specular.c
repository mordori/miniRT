#include "rendering.h"

static float	g_schlick_ggx(float dot, float roughness);

float	reflectance(float ior)
{
	float		reflectance;

	reflectance = (ior - 1.0f) / (ior + 1.0f);
	reflectance *= reflectance;
	return (reflectance);
}

void	specular_probability(t_path *path, t_pixel *pixel, t_vec3 fresnel)
{
	path->p_spec = clampf01((fresnel.r + fresnel.g + fresnel.b) / 3.0f);
	if (path->mat->metallic >= 0.9f)
	{
		path->p_spec = 1.0f;
		path->last_bounce_was_spec = true;
	}
	else if (path->mat->roughness >= 0.9f)
	{
		path->p_spec = 0.0f;
		path->last_bounce_was_spec = false;
	}
	else
	{
		path->p_spec = clampf(path->p_spec, 0.001f, 0.999f);
		path->last_bounce_was_spec = randomf01(pixel->seed) < path->p_spec;
	}
}

bool	bounce_specular(t_path *path, t_vec3 fresnel, float ndotv)
{
	t_vec3		h;
	float		g;
	float		ndotl;
	float		vdoth;
	float		ndoth;
	float		weight;

	h = sample_ggx(path->hit.normal, path->mat->roughness, path->uv);
	path->dir_bounce = vec3_reflect(path->ray.dir, h);
	ndotl = vec3_dot(path->hit.normal, path->dir_bounce);
	if (ndotl <= 0.0f)
		return (false);
	ndoth = clampf01(vec3_dot(path->hit.normal, h));
	vdoth = clampf01(vec3_dot(vec3_negate(path->ray.dir), h));
	g = g_schlick_ggx(ndotv, path->mat->roughness) * g_schlick_ggx(ndotl, path->mat->roughness);
	weight = (g * vdoth) / (ndotv * ndoth + G_EPSILON);
	path->throughput = vec3_mul(path->throughput, fresnel);
	path->throughput = vec3_scale(path->throughput, weight);
	path->throughput = vec3_scale(path->throughput, 1.0f / path->p_spec);
	return (true);
}

// https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
static float	g_schlick_ggx(float dot, float roughness)
{
	float		alpha;
	float		k;

	alpha = roughness * roughness;
	k = alpha / 2.0f;
	return (dot / (dot * (1.0f - k) + k + G_EPSILON));
}
