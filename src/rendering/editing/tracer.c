#include "lib_math.h"
#include "materials.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"

static inline void nee_editing(const t_context* ctx, t_path* path);
static inline void write_to_selection_mask(const t_context* ctx, t_path* path, t_pixel* pixel);

bool trace_ray_editing(const t_context* ctx, t_path* path, t_pixel* pixel) {
	const t_renderer* r = &ctx->renderer;

	bool is_primary_ray = path->bounce == 0;

	bool hitSelectedObj = hit_object(ctx->editor.selected_obj, &path->ray, &path->hit);
	bool hitDirLightObj = hit_object(r->cam.directional_light.obj, &path->ray, &path->hit);
	bool hitBVH = hit_bvh_editing(ctx->scene.geo.bvh_root_idx, &path->ray, &path->hit, ctx->scene.geo.bvh_nodes);
	bool hitPlanes = hit_planes(ctx, &path->ray, &path->hit);

	if (hitSelectedObj || hitDirLightObj || hitBVH || hitPlanes) {
		write_to_selection_mask(ctx, path, pixel);
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (path->mat->is_emissive) {
			t_vec3 light_emission = vec3_mul(path->throughput, path->mat->emission);
			path->color = vec3_add(path->color, light_emission);
			return false;
		}
		if (ctx->scene.env.has_dir_light)
			add_lighting_editing(ctx, path, &r->cam.directional_light);
		if (ctx->scene.env.lights.total > 0)
			nee_editing(ctx, path);
		ambient_lighting(path, &ctx->scene.env.amb_light);
		return false;
	}

	ctx->editor.selection_mask[pixel->y * r->width + pixel->x] = -M_INFf;
	t_vec3 bg_color = background_color(&ctx->scene, &path->ray, r->cam.skydome_uv_offset, is_primary_ray);
	t_vec3 color = vec3_scale(bg_color, ctx->scene.env.amb_light.intensity);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, color));
	return false;
}

static inline void nee_editing(const t_context* ctx, t_path* path) {
	size_t i = 0;
	while (i < ctx->scene.env.lights.total) {
		const t_light* light = ((t_light**)ctx->scene.env.lights.items)[i++];
		add_lighting_editing(ctx, path, light);
	}
}

static inline void write_to_selection_mask(const t_context* ctx, t_path* path, t_pixel* pixel) {
	float* mask = &ctx->editor.selection_mask[pixel->y * ctx->renderer.width + pixel->x];
	if (path->hit.obj == ctx->editor.selected_obj)
		*mask = path->hit.t;
	else
		*mask = -path->hit.t;
}
