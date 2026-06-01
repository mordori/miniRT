#include <stdbool.h>

#include "lib_math.h"
#include "materials.h"
#include "objects.h"
#include "rendering.h"
#include "scene.h"

static inline t_vec3 background_gradient(const t_scene* scene, const float t);

t_vec3 background_color(const t_scene* scene, const t_ray* ray, t_vec2 uv_offset, bool is_primary) {
	bool hide_background = false;

	if (hide_background && is_primary)
		return (t_vec3){ 0 };

	if (!scene->env.skydome.pixels)
		return background_gradient(scene, (ray->dir.y + 1.0f) * 0.5f);

	t_vec2 uv = spherical_uv(ray->dir);
	uv = vec2_add(uv, uv_offset);
	return sample_texture(&scene->env.skydome, uv);
}

static inline t_vec3 background_gradient(const t_scene* scene, const float t) {
	return vec3_lerp(scene->env.amb_light.color, scene->env.amb_color_2, t);
}

bool rotate_skydome(t_context* ctx) {
	static const t_vec3 initial_pos = { { 704000.0f, 484000.0f, 520000.0f, 0.0f } };

	if (!ctx->scene.env.skydome.pixels)
		return false;

	int change = mlx_is_key_down(ctx->mlx, MLX_KEY_PERIOD) - mlx_is_key_down(ctx->mlx, MLX_KEY_COMMA);
	if (change) {
		float delta = 0.085f * fminf(ctx->mlx->delta_time, 0.1f);
		if (change < 0)
			delta = -delta;
		ctx->scene.cam.skydome_uv_offset.u += delta;

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
	}
	return (bool)change;
}
