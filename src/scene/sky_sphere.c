#include "scene.h"
#include "utils.h"

void	init_sky_sphere(t_context *ctx)
{

}

void	add_sky_sphere_tex(t_context *ctx, t_entity entity)
{
	t_texture	*tex;
	ctx->scene.sky_sphere.material.base_color = BASE_TEXTURE;
	tex = mlx_load_png(entity.tex_path);
	if (!tex)
		fatal_error(ctx, "texture load failed");
	ctx->scene.sky_sphere.material.texture = tex;
}
