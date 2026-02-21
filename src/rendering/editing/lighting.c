#include "rendering.h"
#include "materials.h"
#include "utils.h"
#include "scene.h"

static inline t_vec3	direct_lighting_editing(const t_context *ctx, t_path *path, const t_light *light);
static inline t_vec3	add_light(const t_path *path, const t_light *light, float dist);

void	add_lighting_editing(const t_context *ctx, t_path *path, const t_light *light)
{
	t_vec3		radiance;

	radiance = direct_lighting_editing(ctx, path, light);
	radiance = vec3_clamp_mag(radiance, light->max_radiance);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, radiance));
}

static inline t_vec3	direct_lighting_editing(const t_context *ctx, t_path *path, const t_light *light)
{
	t_vec3		l;
	t_vec3		hit_biased;
	float		dist_sq;
	float		dist;
	t_ray		shadow_ray;

	path->n = path->hit.normal;
	hit_biased = vec3_bias(path->hit.point, path->n);
	l = vec3_sub(light->pos, hit_biased);
	dist_sq = vec3_dot(l, l);
	if (dist_sq < LEN_SQ_EPSILON)
		return (vec3_n(0.0f));
	dist = sqrtf(dist_sq);
	path->l = vec3_scale(l, 1.0f / dist);
	path->ndotl = clampf01(vec3_dot(path->n, path->l));
	if (path->ndotl <= G_EPSILON)
		return (vec3_n(0.0f));
	shadow_ray = new_ray(hit_biased, path->l);
	if (!(path->mat->flags & MAT_NO_REC_SHADOW) && hit_shadow(&ctx->scene, &shadow_ray, dist - light->radius - B_EPSILON))
		return (vec3_n(0.0f));
	return (add_light(path, light, dist_sq));
}

void	ambient_lighting(t_path *path, const t_light *light)
{
	t_vec3		res;

	res = vec3_mul(light->color, path->mat->albedo);
	res = vec3_scale(res, light->intensity);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, res));
}

static inline t_vec3	add_light(const t_path *path, const t_light *light, float dist_sq)
{
	t_vec3		res;
	float		attenuation;

	attenuation = light->intensity * path->ndotl / fmax(dist_sq, G_EPSILON);
	res = vec3_mul(path->mat->albedo, light->color);
	res = vec3_scale(res, attenuation * M_1_PI);
	return (res);
}
