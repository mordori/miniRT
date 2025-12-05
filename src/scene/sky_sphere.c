#include "scene.h"
#include "utils.h"

void	init_sky_sphere(t_context *ctx)
{
	ctx->scene.sky_sphere.material.color = (t_color){GREEN};
}

void	add_sky_sphere_tex(t_context *ctx, char *file)
{
	t_texture	*tex;

	if (ctx->scene.sky_sphere.material.texture)
		mlx_delete_texture(ctx->scene.sky_sphere.material.texture);
	ctx->scene.sky_sphere.material.base_color = BASE_TEXTURE;
	tex = mlx_load_png(file);
	if (!tex)
		fatal_error(ctx, "texture load failed");
	ctx->scene.sky_sphere.material.texture = tex;
}
