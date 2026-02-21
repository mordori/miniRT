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
	vector_try_init(ctx, &ctx->scene.geo.objs, false, free);
	vector_try_init(ctx, &ctx->scene.geo.planes, false, free);
	vector_try_init(ctx, &ctx->scene.env.lights, false, free);
	vector_try_init(ctx, &ctx->scene.assets.materials, false, free);
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

	free (ctx->scene.geo.bvh_nodes);
	vector_free(&ctx->scene.geo.objs, &ctx->scene.geo.planes, &ctx->scene.env.lights, &ctx->scene.assets.materials, NULL);
	free_texture(&ctx->scene.env.skydome);
	i = 0;
	while (i < ctx->scene.assets.tex_count)
	{
		if (ctx->scene.assets.textures[i].loaded)
			free_texture(&ctx->scene.assets.textures[i].texture);
		i++;
	}
	ctx->scene.assets.tex_count = 0;
}
