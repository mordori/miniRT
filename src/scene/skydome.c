#include "scene.h"
#include "utils.h"

void	init_skydome(t_context *ctx)
{
	(void)ctx;
}

void	add_skydome_tex(t_context *ctx, char *file)
{
	t_texture	*tex;

	tex = mlx_load_png(file);
	if (!tex)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	ctx->scene.skydome = tex;
}
