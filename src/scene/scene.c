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
	init_directional_light(ctx, &ctx->scene.directional_light);

	if (!parse_scene(ctx, ctx->fd))
		fatal_error(ctx, "Failed to parse scene file", __FILE__, __LINE__);
	close(ctx->fd);
	ctx->fd = ERROR;
	init_bvh(ctx);
}

void	clean_scene(t_context *ctx)
{
	clean_bvh(ctx->scene.bvh_root);
	vector_free(&ctx->scene.objs, &ctx->scene.lights, &ctx->scene.materials, NULL);
	free_texture(&ctx->scene.skydome);
}
