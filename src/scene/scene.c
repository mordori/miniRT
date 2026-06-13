#include "scene.h"

#include "camera.h"
#include "defines.h"
#include "lib_math.h"
#include "lights.h"
#include "materials.h"
#include "objects.h"
#include "utils.h"

void init_scene(t_context* ctx) {
	ctx->scene = (t_scene){ 0 };
	vector_try_init(ctx, &ctx->scene.geo.objs, false, free);
	vector_try_init(ctx, &ctx->scene.env.lights, false, free);
	vector_try_init(ctx, &ctx->scene.assets.materials, false, free);
	lut_srgb_to_linear();

	t_material mat = { //
		.is_emissive = false,
		.emission_color = g_one,
		.emission_strength = 1.0f,
		.emission = g_one,
		.albedo = (t_vec3){ { 0.5f, 0.5f, 0.5f, 1.0f } },
		.roughness = 0.5f,
		.ior = 1.4f,
		.normal_strength = 1.0f
	};
	new_material(ctx, &mat);

	load_mesh_dir(ctx, "assets/models");

	ctx->scene.env.skydome = load_texture(ctx, "assets/textures/sky.png", true);
	ctx->tex_bn = load_texture(ctx, "assets/textures/blue_noise.png", false);

	ctx->bn_stride = (BN_CO_U + ((ctx->scene.env.lights.total + 1) * 2) + 3) & ~3;

	ctx->renderer.render_samples = 64;
	ctx->renderer.render_bounces = 16;

	t_vec3 pos = vec3(0.0f, 1.3f, -2.7f);
	t_vec3 dir = vec3_normalize(vec3(0.0f, -0.33f, 1.0f));
	ctx->scene.cam.exposure = 1.2f;
	ctx->scene.cam.focus_dist = 3.0f;
	init_camera(ctx, pos, dir, 27.0f);

	t_material mat1 = { //
		.is_emissive = true,
		.emission_strength = 1000.0f,
		.emission_color = vec3_n(1.0f),
		.emission = vec3_scale(vec3_n(1.0f), 1000.0f),
		.albedo = g_one,
		.normal_strength = 1.0f,
		.flags = 2
	};
	new_material(ctx, &mat1);

	t_material mat2 = { //
		.is_emissive = false,
		.emission_color = g_one,
		.emission_strength = 1.0f,
		.emission = g_one,
		.albedo = vec3(0.75f, 0.1f, 0.1f),
		.roughness = 0.1f,
		.ior = 1.5f,
		.normal_strength = 1.0f
	};
	new_material(ctx, &mat2);

	t_light light = { //
		.intensity = 1.0f,
		.radius = 30000.0f
	};
	pos = vec3(705000.0f, 485000.0f, 520000.0f);
	init_point_light(ctx, &light, 1, pos);

	ctx->scene.env.ambient = vec3(0.22f, 0.29f, 0.4f);
	ctx->scene.env.gradient_2 = g_one;

	ctx->scene.env.bg_mode = BG_IMAGE;

	add_mesh(ctx, "dragon.obj", 2, false);

	t_vec3 initial_pos = { { 704000.0f, 484000.0f, 520000.0f, 0.0f } };
	ctx->scene.cam.skydome_uv_offset.u = 0.5f;
	t_light* dir_light = &ctx->scene.cam.directional_light;
	float angle = ctx->scene.cam.skydome_uv_offset.u * M_TAUf;
	t_vec2 theta = { .sin = sinf(angle), .cos = cosf(angle) };
	dir_light->obj->transform.pos = (t_vec3){ //
		.x = initial_pos.x * theta.cos + initial_pos.z * theta.sin,
		.y = initial_pos.y,
		.z = -initial_pos.x * theta.sin + initial_pos.z * theta.cos
	};
	update_transform(&dir_light->obj->transform);
	update_bounds(dir_light->obj);

	if (!init_bvh(ctx))
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);

	printf("\n\033[1;32mReady!\033[0m\n");
	printf_init();
}

void clean_scene(t_context* ctx) {
	free(ctx->scene.geo.bvh_nodes);
	if (ctx->scene.env.has_dir_light && ctx->scene.cam.directional_light.obj)
		free(ctx->scene.cam.directional_light.obj);
	vector_free(&ctx->scene.geo.objs, &ctx->scene.env.lights, &ctx->scene.assets.materials, NULL);
	free_texture(&ctx->scene.env.skydome);
	int i = 0;
	while (i < ctx->scene.assets.tex_count) {
		if (ctx->scene.assets.textures[i].loaded)
			free_texture(&ctx->scene.assets.textures[i].texture);
		i++;
	}
	ctx->scene.assets.tex_count = 0;
}
