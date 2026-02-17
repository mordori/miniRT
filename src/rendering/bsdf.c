#include "rendering.h"
#include "materials.h"

// https://google.github.io/filament/main/filament.html#materialsystem/specularbrdf
// https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf

float	power_heuristic(float pdf_d, float pdf_r)
{
	float		d2;
	float		r2;

	d2 = pdf_d * pdf_d;
	r2 = pdf_r * pdf_r;
	return (d2 / fmaxf(d2 + r2, G_EPSILON));
}

void	set_material_data(t_path *path)
{
	t_vec3		albedo;
	t_vec3		f0_dielectric;

	path->n = path->hit.normal;
	path->v = vec3_negate(path->ray.dir);
	path->ndotv = clampf(vec3_dot(path->n, path->v), G_EPSILON, 1.0f);
	path->alpha = fmaxf(path->mat->roughness * path->mat->roughness, 0.001f);
	f0_dielectric = vec3_n(reflectance(path->mat->ior));
	albedo = get_surface_color(path->mat, &path->hit);
	path->f0 = vec3_lerp(f0_dielectric, albedo, path->mat->metallic);
}

void	set_shader_data(t_path *path)
{
	path->h = vec3_normalize(vec3_add(path->v, path->l));
	path->ndotl = clampf01(vec3_dot(path->n, path->l));
	path->ndoth = clampf01(vec3_dot(path->n, path->h));
	path->vdoth = clampf01(vec3_dot(path->v, path->h));
	path->ldoth = clampf01(vec3_dot(path->l, path->h));
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
	pdf_r = pdf_ggx(path);
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
		path->h = sample_ggx(path->n, path->alpha, path->uv);
		path->l = vec3_reflect(path->ray.dir, path->h);
	}
	else
		path->l = sample_cos_hemisphere(path->n, path->uv);
	set_shader_data(path);
	if (path->ndotl <= 1e-12f)
		return (false);
	path->pdf = bsdf_pdf(path);
	path->pdf = fmaxf(path->pdf, G_EPSILON);
	weight = vec3_scale(bsdf(path), path->ndotl / path->pdf);
	path->throughput = vec3_mul(path->throughput, weight);
	return (true);
}
