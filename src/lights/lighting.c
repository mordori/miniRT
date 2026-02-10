#include "lights.h"
#include "materials.h"
#include "objects.h"
#include "utils.h"
#include "rendering.h"

static inline t_vec3	diffuse_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel);
static inline t_vec3	sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float *pdf);

void	add_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	const float		max_brightness = 40.0f;
	t_vec3			lighting;

	lighting = diffuse_lighting(ctx, path, light, pixel);
	if (path->bounce > 0)
		lighting = vec3_clamp_mag(lighting, max_brightness);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, lighting));
}

static inline t_vec3	diffuse_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	t_vec3		res;
	t_vec3		hit_biased;
	t_vec3		n;
	t_vec3		l;
	t_vec3		hit_to_light_center;
	float		dist;
	float		ndotl;
	float		pdf;
	float		t_ca;
	float		ca_dist_sq;
	float		t_hc;

	n = path->hit.normal;
	hit_biased = vec3_bias(path->hit.point, n);
	random_uv(ctx, path, pixel, BN_CO_U);
	hit_to_light_center = vec3_sub(light->pos, hit_biased);
	l = sample_light(hit_to_light_center, light->obj->shape.sphere.radius_sq, path->uv, &pdf);
	if (vec3_dot(l, l) < LEN_SQ_EPSILON)
		return (vec3_n(0.0f));
	ndotl = vec3_dot(n, l);
	if (ndotl <= 0.0f)
		return (vec3_n(0.0f));
	t_ca = vec3_dot(hit_to_light_center, l);
	ca_dist_sq = vec3_dot(hit_to_light_center, hit_to_light_center) - t_ca * t_ca;
	t_hc = sqrtf(fmaxf(0.0f, light->obj->shape.sphere.radius_sq - ca_dist_sq));
	dist = t_ca - t_hc;
	if (!(path->mat->flags & MAT_NO_REC_SHADOW) && hit_shadow(&ctx->scene, hit_biased, l, dist - B_EPSILON))
		return (vec3_n(0.0f));
	res = vec3_mul(light->mat->emission, disney_diffuse(path, n, l, ndotl));
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
