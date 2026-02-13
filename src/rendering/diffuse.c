#include "rendering.h"
#include "materials.h"

static inline float	burley(float ndotv, float ndotl, float ldoth, float roughness);

void	bounce_diffuse(t_path *path)
{
	path->dir_bounce = sample_cos_hemisphere(path->hit.normal, path->uv);
	path->throughput = vec3_mul(path->throughput, get_surface_color(path->mat, &path->hit));
	if (path->mat->metallic > 0.0f)
		path->throughput = vec3_scale(path->throughput, 1.0f - path->mat->metallic);
	path->throughput = vec3_scale(path->throughput, 1.0f / (1.0f - path->p_spec));
}

t_vec3	disney_diffuse(const t_path *path, t_vec3 n, t_vec3 l, float ndotl)
{
	t_vec3		v;
	t_vec3		h;
	float		ndotv;
	float		ldoth;
	t_vec3		f0;
	t_vec3		f;
	t_vec3		f_d;
	t_vec3		k_d;
	float		factor;

	v = vec3_negate(path->ray.dir);
	h = vec3_normalize(vec3_add(v, l));
	ndotv = clampf(vec3_dot(n, v), G_EPSILON, 1.0f);
	ndotl = clampf01(ndotl);
	ldoth = clampf01(vec3_dot(l, h));
	f0 = vec3_lerp(vec3_n(0.04f), get_surface_color(path->mat, &path->hit), path->mat->metallic);
	f = vec3_schlick(f0, ldoth);
	k_d = vec3_sub(vec3_n(1.0f), f);
	factor = burley(ndotv, ndotl, ldoth, path->mat->roughness);
	f_d = vec3_mul(get_surface_color(path->mat, &path->hit), k_d);
	f_d = vec3_scale(f_d, (1.0f - path->mat->metallic) * factor);
	return (f_d);
}

static inline float	burley(float ndotv, float ndotl, float ldoth, float roughness)
{
	float		f90;
	float		l_scatter;
	float		v_scatter;

	f90 = 0.5f + 2.0f * roughness * ldoth * ldoth;
	l_scatter = f_schlick(ndotl, 1.0f, f90);
	v_scatter = f_schlick(ndotv, 1.0f, f90);
	return (l_scatter * v_scatter * M_1_PI);
}
