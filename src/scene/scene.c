#include "scene.h"

#include <stdbool.h>
#include <string.h>

#include "defines.h"
#include "lib_math.h"
#include "libft_str.h"
#include "materials.h"
#include "objects.h"
#include "parsing.h"
#include "utils.h"

void init_scene(t_context* ctx) {
	ctx->scene = (t_scene){ 0 };
	vector_try_init(ctx, &ctx->scene.geo.objs, false, free);
	vector_try_init(ctx, &ctx->scene.geo.planes, false, free);
	vector_try_init(ctx, &ctx->scene.env.lights, false, free);
	vector_try_init(ctx, &ctx->scene.assets.materials, false, free);
	lut_srgb_to_linear();

	t_material mat = { 0 };
	mat.albedo = (t_vec3){ { 0.5f, 0.5f, 0.5f, 1.0f } };
	mat.ior = 1.4f;
	mat.roughness = 0.4f;
	new_material(ctx, &mat, 0);

	// TODO: remove hard coded test
	if (strcmp("assets/scenes/empty.rt", ctx->file) == 0 || strcmp("assets/scenes/cornell_box.rt", ctx->file) == 0) {
		load_mesh_dir(ctx, "assets/models");
		printf("\n");
	}

	if (!parse_scene(ctx, ctx->fd))
		fatal_error(ctx, "Failed to parse scene file", __FILE__, __LINE__);
	ctx->tex_bn = load_texture("assets/textures/blue_noise.png", false);
	if (!ctx->tex_bn.pixels)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	ctx->bn_stride = (BN_CO_U + ((ctx->scene.env.lights.total + 1) * 2) + 3) & ~3;

	close(ctx->fd);
	ctx->fd = ERROR;

	// TODO: remove hard coded test
	{
		if (strcmp("assets/scenes/empty.rt", ctx->file) == 0)
			add_mesh(ctx, "dragon.obj", 4, false);
		else if (strcmp("assets/scenes/cornell_box.rt", ctx->file) == 0)
			add_mesh(ctx, "bunny.obj", 5, false);

		if (ctx->scene.env.has_dir_light) {
			t_vec3 initial_pos = { { 704000.0f, 484000.0f, 520000.0f, 0.0f } };
			ctx->scene.cam.skydome_uv_offset.u = 0.5f;
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

	if (!init_bvh(ctx))
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);

	printf("\n\033[1;32mReady!\033[0m\n");
	printf_init();
}

void clean_scene(t_context* ctx) {
	free(ctx->scene.geo.bvh_nodes);
	if (ctx->scene.env.has_dir_light && ctx->scene.cam.directional_light.obj)
		free(ctx->scene.cam.directional_light.obj);
	vector_free(&ctx->scene.geo.objs, &ctx->scene.geo.planes, &ctx->scene.env.lights, &ctx->scene.assets.materials, NULL);
	free_texture(&ctx->scene.env.skydome);
	int i = 0;
	while (i < ctx->scene.assets.tex_count) {
		if (ctx->scene.assets.textures[i].loaded)
			free_texture(&ctx->scene.assets.textures[i].texture);
		i++;
	}
	ctx->scene.assets.tex_count = 0;
}

void validate_file_type(char* file) {
	char* dot = ft_strchr(file, '.');
	if (!dot || ft_strcmp(dot, ".rt") != 0)
		fatal_error(NULL, "invalid file format", __FILE__, __LINE__);
}
