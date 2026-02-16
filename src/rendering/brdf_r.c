#include "rendering.h"

static inline float	d_ggx(float ndoth, float a);
static inline float	v_hammon(float ndotv, float ndotl, float alpha);

float	reflectance(float ior)
{
	float		reflectance;

	reflectance = (ior - 1.0f) / (ior + 1.0f);
	reflectance *= reflectance;
	return (reflectance);
}

t_vec3	cook_torrance_brdf(const t_path *path)
{
	float		d;
	float		v;
	t_vec3		f;

	d = d_ggx(path->ndoth, path->alpha);
	v = v_hammon(path->ndotv, path->ndotl, path->alpha);
	f = vec3_schlick(path->f0, path->vdoth);
	return (vec3_scale(f, d * v));
}

float	pdf_ggx(const t_path *path)
{
	float		d;
	float		pdf_h;
	float		jacobian;

	d = d_ggx(path->ndoth, path->alpha);
	pdf_h = d * path->ndoth;
	jacobian = 4.0f * path->vdoth;
	return (pdf_h / fmaxf(jacobian, G_EPSILON));
}

static inline float	d_ggx(float ndoth, float a)
{
	float		a2;
	float		den;

	a2 = a * a;
	den = (ndoth * ndoth) * (a2 - 1.0f) + 1.0f;
	den = M_PI * (den * den);
	return (a2 / den);
}

static inline float	v_hammon(float ndotv, float ndotl, float alpha)
{
	float		a;
	float		b;

	a = 2.0f * ndotl * ndotv;
	b = ndotl + ndotv;
	return (0.5f / (ft_lerp_fast(a, b, alpha) + G_EPSILON));
}
