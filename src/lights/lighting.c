#include "lights.h"
#include "objects.h"
#include "utils.h"

static inline t_vec3	sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float *pdf);
static inline t_vec3	brdf(const t_path *path, t_vec3 n, t_vec3 l, float ndotl);
static inline float	f_d_burley(float ndotv, float ndotl, float ldoth, float roughness);

t_vec3	compute_lighting(const t_context *ctx, const t_path *path, const t_light *light, t_pixel *pixel)
{
	t_vec3		res;
	t_vec3		hit_biased;
	t_vec3		n;
	t_vec3		l;
	t_vec3		hit_to_light_center;
	t_vec2		uv;
	float		dist;
	float		ndotl;
	float		pdf;
	float		t_ca;
	float		ca_dist_sq;
	float		t_hc;

	n = path->hit.normal;
	hit_biased = vec3_add(path->hit.point, vec3_scale(n, B_EPSILON));
	if (path->bounce == 0)
		uv = vec2(blue_noise(&ctx->tex_bn, pixel, BN_CO_U), blue_noise(&ctx->tex_bn, pixel, BN_CO_V));
	else
		uv = vec2(randomf01(pixel->seed), randomf01(pixel->seed));
	hit_to_light_center = vec3_sub(light->pos, hit_biased);
	l = sample_light(hit_to_light_center, light->obj->shape.sphere.radius_sq, uv, &pdf);
	if (vec3_dot(l, l) < LEN_SQ_EPSILON)
		return (vec3_n(0.0f));
	ndotl = vec3_dot(n, l);
	if (ndotl <= 0.0f)
		return (vec3_n(0.0f));
	t_ca = vec3_dot(hit_to_light_center, l);
	ca_dist_sq = vec3_dot(hit_to_light_center, hit_to_light_center) - t_ca * t_ca;
	t_hc = sqrtf(fmaxf(0.0f, light->obj->shape.sphere.radius_sq - ca_dist_sq));
	dist = t_ca - t_hc;
	if (hit_shadow(&ctx->scene, hit_biased, l, dist - B_EPSILON))
		return (vec3_n(0.0f));
	res = vec3_mul(light->mat->emission, brdf(path, n, l, ndotl));
	res = vec3_scale(res, ndotl / pdf);
	return (res);
}

static inline t_vec3	sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float *pdf)
{
	float		dist_sq;
	float		cos_theta_max;
	t_vec3		res;
	t_vec3		l_dir;

	dist_sq = vec3_dot(l, l);
	if (dist_sq < LEN_SQ_EPSILON || dist_sq <= radius_sq)
		return (vec3_n(0.0f));
	cos_theta_max = sqrtf(fmaxf(0.0f, 1.0f - (radius_sq / dist_sq)));
	*pdf = 1.0f / (M_TAU * (1.0f - cos_theta_max));
	l_dir = vec3_scale(l, 1.0f / sqrtf(dist_sq));
	res = sample_cone(l_dir, cos_theta_max, uv);
	return (res);
}

static inline float	f_d_burley(float ndotv, float ndotl, float ldoth, float roughness)
{
	float		f90;
	float		l_scatter;
	float		v_scatter;

	f90 = 0.5f + 2.0f * roughness * ldoth * ldoth;
	l_scatter = f_schlick(ndotl, 1.0f, f90);
	v_scatter = f_schlick(ndotv, 1.0f, f90);
	return (l_scatter * v_scatter * M_1_PI);
}

static inline t_vec3	brdf(const t_path *path, t_vec3 n, t_vec3 l, float ndotl)
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
	f0 = vec3_lerp(vec3_n(0.04f), path->mat->albedo, path->mat->metallic);
	f = vec3_schlick(f0, ldoth);
	k_d = vec3_sub(vec3_n(1.0f), f);
	factor = f_d_burley(ndotv, ndotl, ldoth, path->mat->roughness);
	f_d = vec3_mul(path->mat->albedo, k_d);
	f_d = vec3_scale(f_d, (1.0f - path->mat->metallic) * factor);
	return (f_d);
}
