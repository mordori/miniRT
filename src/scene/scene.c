#include "scene.h"
#include "camera.h"
#include "lights.h"
#include "objects.h"
#include "parsing.h"
#include "libft_io.h"
#include "utils.h"
#include "materials.h"
#include "rendering.h"

void	init_scene(t_context *ctx)
{
	ctx->scene = (t_scene){0};
	vector_try_init(ctx, &ctx->scene.objs, false, free);
	vector_try_init(ctx, &ctx->scene.lights, false, free);
	vector_try_init(ctx, &ctx->scene.materials, false, free);
	if (!parse_scene(ctx, ctx->fd))
		fatal_error(ctx, "Failed to parse scene file", __FILE__, __LINE__);
	ctx->tex_bn = load_texture("assets/textures/blue_noise.png", false);
	if (!ctx->tex_bn.pixels)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	printf_init(ctx);
	close(ctx->fd);
	ctx->fd = ERROR;
	init_bvh(ctx);
}

void	clean_scene(t_context *ctx)
{
	int	i;

	clean_bvh(ctx->scene.bvh_root);
	vector_free(&ctx->scene.objs, &ctx->scene.lights, &ctx->scene.materials, NULL);
	free_texture(&ctx->scene.skydome);
	i = 0;
	while (i < ctx->scene.tex_count)
	{
		if (ctx->scene.textures[i].loaded)
			free_texture(&ctx->scene.textures[i].texture);
		i++;
	}
	ctx->scene.tex_count = 0;
}
