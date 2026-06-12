#include <stdbool.h>

#include "defines.h"
#include "lib_math.h"
#include "materials.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"

t_vec3 background_color(const t_scene* scene, const t_ray* ray, t_vec2 uv_offset, bool is_primary, bool show_background) {
	if (!show_background && is_primary)
		return g_zero;

	switch (scene->env.bg_mode) {
		case BG_SOLID: return scene->env.ambient_1;
		case BG_GRADIENT: return vec3_lerp(scene->env.ambient_1, scene->env.ambient_2, (ray->dir.y + 1.0f) * 0.5f);
		case BG_IMAGE: {
			t_vec2 uv = vec2_add(spherical_uv(ray->dir), uv_offset);
			return sample_texture(&scene->env.skydome, uv);
		}
		default: break;
	}
}

bool rotate_skydome(t_context* ctx) {
	static const t_vec3 initial_pos = { { 704000.0f, 484000.0f, 520000.0f, 0.0f } };

	if (ctx->scene.env.bg_mode != BG_IMAGE)
		return false;

	if (ctx->scene.env.has_dir_light) {
		t_light* light = &ctx->scene.cam.directional_light;
		float angle = ctx->scene.cam.skydome_uv_offset.u * M_TAUf;
		t_vec2 theta = { .sin = sinf(angle), .cos = cosf(angle) };
		light->obj->transform.pos = (t_vec3){ //
			.x = initial_pos.x * theta.cos + initial_pos.z * theta.sin,
			.y = initial_pos.y,
			.z = -initial_pos.x * theta.sin + initial_pos.z * theta.cos
		};
		update_transform(&light->obj->transform);
		update_bounds(light->obj);
	}
	return true;
}
