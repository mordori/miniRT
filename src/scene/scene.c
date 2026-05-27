#include "scene.h"

#include "defines.h"
#include "libft_str.h"
#include "parsing.h"
#include "utils.h"

void init_scene(t_context* ctx) {
	ctx->scene = (t_scene){ 0 };
	vector_try_init(ctx, &ctx->scene.geo.objs, false, free);
	vector_try_init(ctx, &ctx->scene.geo.planes, false, free);
	vector_try_init(ctx, &ctx->scene.env.lights, false, free);
	vector_try_init(ctx, &ctx->scene.assets.materials, false, free);
	lut_srgb_to_linear();

	if (!parse_scene(ctx, ctx->fd))
		fatal_error(ctx, "Failed to parse scene file", __FILE__, __LINE__);
	ctx->tex_bn = load_texture("assets/textures/blue_noise.png", false);
	if (!ctx->tex_bn.pixels)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	ctx->bn_stride = (BN_CO_U + ((ctx->scene.env.lights.total + 1) * 2) + 3) & ~3;
	printf_init();
	close(ctx->fd);
	ctx->fd = ERROR;

	if (!init_bvh(ctx))
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
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
