#include "rendering.h"
#include "materials.h"

float	power_heuristic(float a, float b)
{
	float		a2;
	float		b2;

	a2 = a * a;
	b2 = b * b;
	return (a2 / fmaxf(a2 + b2, G_EPSILON));
}

t_vec3	bsdf(t_path *path)
{
	t_vec3		diffuse;
	t_vec3		specular;

	diffuse = disney_brdf(path);
	specular = cook_torrance_brdf(path);
	return (vec3_add(diffuse, specular));
}

float	bsdf_pdf(t_path *path)
{
	float		pdf_d;
	float		pdf_r;
	float		pdf;

	pdf_d = pdf_cos(path->ndotl);
	pdf_r = pdf_ggx_vndf(path);
	if (path->mat->metallic >= 0.9f)
		pdf = pdf_r;
	else
		pdf = (path->p_spec * pdf_r) + ((1.0f - path->p_spec) * pdf_d);
	return (pdf);
}

bool	sample_bsdf(t_path *path)
{
	t_vec3		weight;

	if (path->sample_spec)
	{
		sample_ggx_vndf(path, path->alpha, path->uv);
		path->l = vec3_reflect(path->ray.dir, path->h);
	}
	else
		path->l = sample_cos_hemisphere(path->n, path->uv);
	set_shader_data(path);
	if (path->ndotl <= LEN_SQ_EPSILON)
		return (false);
	path->pdf = bsdf_pdf(path);
	weight = vec3_scale(bsdf(path), path->ndotl / path->pdf);
	weight = vec3_clamp_mag(weight, CLAMP_INDIRECT);
	path->throughput = vec3_mul(path->throughput, weight);
	return (true);
}
