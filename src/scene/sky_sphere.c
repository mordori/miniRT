#include "scene.h"
#include "utils.h"

void	init_sky_sphere(t_context *ctx)
{
	(void)ctx;
}

void	add_sky_sphere_tex(t_context *ctx, char *file)
{
	t_texture	*tex;

	if (ctx->scene.sky_dome)
		mlx_delete_texture(ctx->scene.sky_dome);
	tex = mlx_load_png(file);
	if (!tex)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	ctx->scene.sky_dome = tex;
}
