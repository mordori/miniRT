#include "rendering.h"
#include "materials.h"
#include "utils.h"
#include "scene.h"

static inline t_vec3	direct_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel);
static inline t_vec3	sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float *pdf);

void	add_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	t_vec3		radiance;

	radiance = direct_lighting(ctx, path, light, pixel);
	radiance = vec3_clamp_mag(radiance, light->max_brightness);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, radiance));
}

static inline t_vec3	direct_lighting(const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	t_ray		shadow_ray;
	t_vec3		radiance;
	t_vec3		hit_biased;
	t_vec3		hit_to_light_center;
	float		dist;
	float		t_ca;
	float		ca_dist_sq;
	float		t_hc;
	float		weight;

	random_uv(ctx, path, pixel, BN_CO_U);
	hit_biased = vec3_bias(path->hit.point, path->n);
	hit_to_light_center = vec3_sub(light->pos, hit_biased);
	path->l = sample_light(hit_to_light_center, light->obj->shape.sphere.radius_sq, path->uv, &path->pdf);
	if (vec3_dot(path->l, path->l) < LEN_SQ_EPSILON)
		return (vec3_n(0.0f));
	set_shader_data(path);
	if (path->ndotl <= G_EPSILON)
		return (vec3_n(0.0f));
	t_ca = vec3_dot(hit_to_light_center, path->l);
	ca_dist_sq = vec3_dot(hit_to_light_center, hit_to_light_center) - t_ca * t_ca;
	t_hc = sqrtf(fmaxf(0.0f, light->obj->shape.sphere.radius_sq - ca_dist_sq));
	dist = t_ca - t_hc;
	shadow_ray = new_ray(hit_biased, path->l);
	if (!(path->mat->flags & MAT_NO_REC_SHADOW) && hit_shadow(&ctx->scene, &shadow_ray, dist - B_EPSILON, path->hit.obj))
		return (vec3_n(0.0f));
	weight = power_heuristic(path->pdf, bsdf_pdf(path));
	radiance = vec3_mul(light->mat->emission, bsdf(path));
	radiance = vec3_scale(radiance, (path->ndotl / path->pdf) *  weight);
	return (radiance);
}

float	light_pdf(t_vec3 l, float radius_sq)
{
	float		dist_sq;
	float		sin_theta_sq;
	float		cos_theta_max;
	float		pdf;

	dist_sq = vec3_dot(l, l);
	sin_theta_sq = radius_sq / dist_sq;
	sin_theta_sq = fminf(sin_theta_sq, 0.9999f);
	cos_theta_max = sqrtf(1.0f - sin_theta_sq);
	pdf = 1.0f / fmaxf((M_TAU * (1.0f - cos_theta_max)), G_EPSILON);
	return (pdf);
}

static inline t_vec3	sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float *pdf)
{
	float		dist_sq;
	float		sin_theta_sq;
	float		cos_theta_max;
	t_vec3		res;
	t_vec3		l_dir;

	dist_sq = vec3_dot(l, l);
	if (dist_sq <= radius_sq)
		return (vec3_n(0.0f));
	l_dir = vec3_scale(l, 1.0f / sqrtf(dist_sq));
	sin_theta_sq = radius_sq / dist_sq;
	sin_theta_sq = fminf(sin_theta_sq, 0.9999f);
	cos_theta_max = sqrtf(1.0f - sin_theta_sq);
	*pdf = 1.0f / fmaxf((M_TAU * (1.0f - cos_theta_max)), G_EPSILON);
	res = sample_cone(l_dir, cos_theta_max, uv);
	return (res);
}

bool	hit_shadow(const t_scene *scene, const t_ray *ray, float dist, const t_object *skip)
{
	return (hit_bvh_shadow(scene->bvh_root, ray, dist, skip));
}
