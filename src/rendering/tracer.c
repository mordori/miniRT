#include <stdint.h>

#include "camera.h"
#include "defines.h"
#include "lib_math.h"
#include "materials.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"
#include "utils.h"

static inline bool trace_ray(const t_context* ctx, t_path* path, t_pixel* pixel, t_render_mode mode);
static inline void nee(const t_context* ctx, t_path* path, t_pixel* pixel, t_render_mode mode);
static inline bool scatter(const t_context* ctx, t_path* path, t_pixel* pixel);
static inline bool evaluate_emissive(const t_context* ctx, t_path* path, t_render_mode mode);
static inline bool russian_roulette(t_path* path, t_pixel* pixel);

t_vec3 trace_path(const t_context* ctx, t_pixel* pixel, t_render_mode mode, uint8_t bounces) {
	const t_renderer* r = &ctx->renderer;

	t_vec3 pixel_loc = vec3_add(vec3_scale(r->cam.viewport.d_u, pixel->u), vec3_scale(r->cam.viewport.d_v, pixel->v));
	pixel_loc = vec3_add(r->cam.viewport.pixel_00_loc, pixel_loc);
	t_vec3 pixel_dir = vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos));
	t_vec3 focus_plane = vec3_scale(pixel_dir, (r->cam.focus_dist / vec3_dot(pixel_dir, r->cam.forward)));

	t_vec3 ray_orig = sample_defocus_disk(ctx, pixel);
	t_path path = { //
		.ray = new_ray(ray_orig, vec3_normalize(vec3_sub(vec3_add(r->cam.transform.pos, focus_plane), ray_orig))),
		.throughput = (t_vec3){ .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f }
	};
	while (path.bounce < bounces) {
		path.hit = new_hit(path.bounce);
		if (mode == SOLID && !trace_ray_editing(ctx, &path, pixel))
			break;
		else if (mode != SOLID && !trace_ray(ctx, &path, pixel, mode))
			break;
	}
	if (vec3_is_nan_inf(path.color))
		return (t_vec3){ 0 };
	return path.color;
}

static inline void accumulate_aux_buffers(const t_renderer* r, const t_pixel* p, t_vec3 albedo, t_vec3 normal) {
	uint32_t i = p->y * r->width + p->x;
	if (r->frame == 1) {
		r->albedo_buffer[i] = albedo;
		r->normal_buffer[i] = normal;
	} else {
		r->albedo_buffer[i] = vec3_add(r->albedo_buffer[i], albedo);
		r->normal_buffer[i] = vec3_add(r->normal_buffer[i], normal);
	}
}

static inline bool trace_ray(const t_context* ctx, t_path* path, t_pixel* pixel, t_render_mode mode) {
	const t_renderer* r = &ctx->renderer;

	bool is_primary_ray = path->bounce == 0;

	bool hitDirLightObj = false;
	if (ctx->scene.env.has_dir_light) {
		t_object* dir_light = ctx->renderer.cam.directional_light.obj;
		hitDirLightObj = !(is_primary_ray && (dir_light->flags & OBJ_HIDDEN_CAM)) && hit_object(dir_light, &path->ray, &path->hit);
	}
	bool hitBVH = hit_bvh(ctx->scene.geo.bvh_root_idx, &path->ray, &path->hit, ctx->scene.geo.bvh_nodes);

	if (hitBVH || hitDirLightObj) {
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (is_primary_ray) {
			t_vec3 albedo = get_surface_color(path->mat, &path->hit);
			t_vec3 normal = path->n;
			accumulate_aux_buffers(r, pixel, albedo, normal);
		}
		if (evaluate_emissive(ctx, path, mode))
			return false;
		if (ctx->scene.env.has_dir_light)
			path->color = vec3_add(path->color, add_lighting(ctx, path, &ctx->renderer.cam.directional_light, pixel));
		if (ctx->scene.env.lights.total > 0)
			nee(ctx, path, pixel, mode);
		return scatter(ctx, path, pixel);
	}
	t_vec3 bg_color =
		background_color(&ctx->scene, &path->ray, ctx->renderer.cam.skydome_uv_offset, is_primary_ray, ctx->scene.env.show_background);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	if (is_primary_ray)
		accumulate_aux_buffers(r, pixel, bg_color, vec3_negate(path->ray.dir));
	return false;
}

static inline bool evaluate_emissive(const t_context* ctx, t_path* path, t_render_mode mode) {
	if (!path->mat->is_emissive)
		return false;

	t_vec3 light_emission;
	if (path->bounce == 0) {
		light_emission = path->mat->emission;
	} else {
		float pdf = light_pdf(vec3_sub(path->ray.origin, path->hit.obj->transform.pos), path->hit.obj->shape.sphere.radius_sq);
		if (mode == PREVIEW && ctx->scene.env.lights.total > 0)
			pdf /= (float)ctx->scene.env.lights.total;
		float weight = power_heuristic(path->pdf, pdf);
		light_emission = vec3_mul(path->throughput, vec3_scale(path->mat->emission, weight));
		light_emission = vec3_clamp_mag(light_emission, MAX_RADIANCE);
	}
	path->color = vec3_add(path->color, light_emission);
	return true;
}

static inline void nee(const t_context* ctx, t_path* path, t_pixel* pixel, t_render_mode mode) {
	if (mode == PREVIEW) {
		float random_01;
		uint32_t li_dim = BN_LI + (path->bounce * ctx->bn_stride);
		if (path->bounce == 0)
			random_01 = blue_noise(&ctx->tex_bn, pixel, li_dim);
		else
			random_01 = r1_sequence(pixel->frame + (path->bounce * FP_PRIME), static_blue_noise(&ctx->tex_bn, pixel, li_dim));
		size_t i = random_01 * ctx->scene.env.lights.total;
		if (i >= ctx->scene.env.lights.total)
			i = ctx->scene.env.lights.total - 1;
		const t_light* light = ((t_light**)ctx->scene.env.lights.items)[i];
		path->color = vec3_add(path->color, vec3_scale(add_lighting(ctx, path, light, pixel), (float)ctx->scene.env.lights.total));
		return;
	}

	size_t i = 0;
	while (i < ctx->scene.env.lights.total) {
		const t_light* light = ((t_light**)ctx->scene.env.lights.items)[i++];
		path->color = vec3_add(path->color, add_lighting(ctx, path, light, pixel));
	}
}

static inline bool scatter(const t_context* ctx, t_path* path, t_pixel* pixel) {
	t_vec3 f = vec3_schlick(path->f0, path->ndotv);
	float p = fmaxf(fmaxf(f.r, f.g), f.b);
	path->p_spec = clampf(p, 0.1f, 0.9f);
	if (path->mat->metallic >= 0.9f)
		path->p_spec = 1.0f;

	float random_01;
	uint32_t spec_dim = BN_SR_U + (path->bounce * ctx->bn_stride);
	if (path->bounce == 0)
		random_01 = blue_noise(&ctx->tex_bn, pixel, spec_dim);
	else
		random_01 = r1_sequence(pixel->frame + (path->bounce * FP_PRIME), static_blue_noise(&ctx->tex_bn, pixel, spec_dim));
	path->sample_spec = random_01 <= path->p_spec;

	random_uv(ctx, path, pixel, BN_SC_U);
	if (!sample_bsdf(path))
		return false;
	path->ray = new_ray(vec3_bias(path->hit.point, path->hit.normal), path->l);
	return russian_roulette(path, pixel);
}

static inline bool russian_roulette(t_path* path, t_pixel* pixel) {
	if (path->bounce >= DEPTH_ENABLE_RR) {
		float p = fmaxf(path->throughput.r, fmaxf(path->throughput.g, path->throughput.b));
		p = clampf(p, 0.05f, 0.95f);
		if (randomf01(pixel->seed) > p)
			return false;
		path->throughput = vec3_scale(path->throughput, 1.0f / p);
	}
	++path->bounce;
	return true;
}
