#include "utils.h"

// void	random_uv(const t_context *ctx, t_path *path, t_pixel *pixel, t_bn_channel c)
// {
// 	if (path->bounce == 0)
// 		path->uv = vec2(blue_noise(&ctx->tex_bn, pixel, c), blue_noise(&ctx->tex_bn, pixel, c + 1));
// 	else
// 		path->uv = vec2(randomf01(pixel->seed), randomf01(pixel->seed));
// }

void	random_uv(const t_context *ctx, t_path *path, t_pixel *pixel, t_bn_channel c)
{
	t_vec2	offset;

	offset = vec2(blue_noise(&ctx->tex_bn, pixel, c),
			blue_noise(&ctx->tex_bn, pixel, c + 1));
	if (path->bounce == 0)
		path->uv = offset;
	else
		path->uv = r2_sequence(path->bounce, offset);
}
