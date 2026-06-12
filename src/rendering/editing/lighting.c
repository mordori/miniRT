#include "defines.h"
#include "lib_math.h"
#include "materials.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"
#include "utils.h"

static inline t_vec3 direct_lighting_editing(const t_context* ctx, t_path* path, const t_light* light);
static inline t_vec3 add_light(const t_path* path, const t_light* light, float dist);
static inline bool hit_shadow(const t_context* ctx, const t_path* path, t_vec3 hit_biased, float dist);

void add_lighting_editing(const t_context* ctx, t_path* path, const t_light* light) {
	t_vec3 radiance = direct_lighting_editing(ctx, path, light);
	radiance = vec3_clamp_mag(radiance, light->max_radiance);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, radiance));
}

static inline t_vec3 direct_lighting_editing(const t_context* ctx, t_path* path, const t_light* light) {
	path->n = path->hit.normal;

	t_vec3 l = vec3_sub(light->obj->transform.pos, path->hit.point);
	float dist_sq = vec3_dot(l, l);
	if (dist_sq < LEN_SQ_EPSILON)
		return g_zero;

	float dist = sqrtf(dist_sq);
	path->l = vec3_scale(l, 1.0f / dist);

	path->ndotl = clampf01(vec3_dot(path->n, path->l));
	if (path->ndotl <= G_EPSILON)
		return g_zero;

	t_vec3 abs_p = vec3_fabsf(path->hit.point);
	float max_coord = fmaxf(fmaxf(abs_p.x, abs_p.y), abs_p.z);
	float bias = fmaxf(B_EPSILON, max_coord * G_EPSILON);
	float adaptive_bias = bias * fmaxf(1.0f, 1.0f / fmaxf(path->ndotl, 0.05f));
	t_vec3 hit_biased = vec3_add(path->hit.point, vec3_scale(path->hit.geo_normal, adaptive_bias));

	dist = dist - light->radius;
	if (hit_shadow(ctx, path, hit_biased, dist - fmaxf(B_EPSILON, dist * G_EPSILON)))
		return g_zero;

	return add_light(path, light, dist_sq);
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

void ambient_lighting(t_path* path, const t_light* light) {
	t_vec3 radiance = vec3_mul(light->emission, get_surface_color(path->mat, &path->hit));
	radiance = vec3_scale(radiance, light->intensity);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, radiance));
}

static inline t_vec3 add_light(const t_path* path, const t_light* light, float dist_sq) {
	float attenuation;

	attenuation = path->ndotl * (light->radius_sq / fmax(dist_sq, G_EPSILON));

	t_vec3 albedo = get_surface_color(path->mat, &path->hit);
	t_vec3 radiance = vec3_mul(albedo, light->emission);
	return vec3_scale(radiance, attenuation);
}
