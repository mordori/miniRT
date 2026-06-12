#include "materials.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"
#include "utils.h"

static inline t_vec3 evaluate_light(const t_context* ctx, t_path* path, const t_light* light, t_pixel* pixel);
static inline t_vec3 sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float* pdf);
static inline bool hit_shadow(const t_context* ctx, const t_path* path, t_vec3 hit_biased, float dist);

t_vec3 add_lighting(const t_context* ctx, t_path* path, const t_light* light, t_pixel* pixel) {
	t_vec3 radiance = evaluate_light(ctx, path, light, pixel);
	if (path->bounce > 0)
		radiance = vec3_clamp_mag(radiance, light->max_radiance);
	return vec3_mul(path->throughput, radiance);
}

static inline t_vec3 evaluate_light(const t_context* ctx, t_path* path, const t_light* light, t_pixel* pixel) {
	random_uv(ctx, path, pixel, BN_CO_U + (light->idx * 2u));

	t_vec3 hit_to_l_center = vec3_sub(light->obj->transform.pos, path->hit.point);
	path->l = sample_light(hit_to_l_center, light->radius_sq, path->uv, &path->pdf);

	set_shader_data(path);
	if (path->ndotl <= G_EPSILON)
		return g_zero;

	t_vec3 abs_p = vec3_fabsf(path->hit.point);
	float max_coord = fmaxf(fmaxf(abs_p.x, abs_p.y), abs_p.z);
	float bias = fmaxf(B_EPSILON, max_coord * G_EPSILON);
	float adaptive_bias = bias * fmaxf(1.0f, 1.0f / fmaxf(path->ndotl, 0.05f));
	t_vec3 hit_biased = vec3_add(path->hit.point, vec3_scale(path->hit.geo_normal, adaptive_bias));

	t_vec3 biased_to_l_center = vec3_sub(light->obj->transform.pos, hit_biased);
	t_vec3 cross = vec3_cross(biased_to_l_center, path->l);
	float dist = vec3_dot(biased_to_l_center, path->l) - sqrtf(fmaxf(0.0f, light->radius_sq - vec3_dot(cross, cross)));

	if (hit_shadow(ctx, path, hit_biased, dist - fmaxf(B_EPSILON, dist * G_EPSILON)))
		return g_zero;

	t_vec3 radiance = vec3_mul(light->emission, bsdf(path));
	radiance = vec3_scale(radiance, (path->ndotl / path->pdf) * power_heuristic(path->pdf, bsdf_pdf(path)));
	return radiance;
}

static inline t_vec3 sample_light(t_vec3 l, float radius_sq, t_vec2 uv, float* pdf) {
	float dist_sq = vec3_dot(l, l);
	if (dist_sq <= radius_sq)
		return g_zero;

	t_vec3 l_dir = vec3_scale(l, 1.0f / sqrtf(dist_sq));

	float sin_theta_sq = radius_sq / dist_sq;
	sin_theta_sq = fminf(sin_theta_sq, 0.9999f);

	float cos_theta_max;
	float solid_angle;
	if (sin_theta_sq < 1e-3f) {
		cos_theta_max = 1.0f - (sin_theta_sq * 0.5f);
		solid_angle = M_PIf * sin_theta_sq;
	} else {
		cos_theta_max = sqrtf(1.0f - sin_theta_sq);
		solid_angle = M_TAUf * (1.0f - cos_theta_max);
	}
	*pdf = 1.0f / fmaxf(solid_angle, G_EPSILON);
	return sample_cone(l_dir, cos_theta_max, uv);
}

float light_pdf(t_vec3 l, float radius_sq) {
	float dist_sq = vec3_dot(l, l);
	if (dist_sq <= radius_sq)
		return (0.0f);

	float sin_theta_sq = radius_sq / dist_sq;
	sin_theta_sq = fminf(sin_theta_sq, 0.9999f);

	float solid_angle;
	if (sin_theta_sq < 1e-3f) {
		solid_angle = M_PIf * sin_theta_sq;
	} else {
		float cos_theta_max = sqrtf(1.0f - sin_theta_sq);
		solid_angle = M_TAUf * (1.0f - cos_theta_max);
	}
	return 1.0f / fmaxf(solid_angle, G_EPSILON);
}

static inline bool hit_shadow(const t_context* ctx, const t_path* path, t_vec3 hit_biased, float dist) {
	t_ray shadow_ray = new_ray(hit_biased, path->l);
	t_hit dummy_hit = { .t = dist };
	if (!(path->mat->flags & MAT_NO_REC_SHADOW) &&
		((ctx->editor.selected_obj && !(ctx->editor.selected_obj->flags & OBJ_NO_CAST_SHADOW) &&
			 hit_object(ctx->editor.selected_obj, &shadow_ray, &dummy_hit)) ||
			hit_bvh_shadow(ctx->scene.geo.bvh_root_idx, &shadow_ray, dist, ctx->scene.geo.bvh_nodes))) {
		return true;
	}
	return false;
}
