#include "scene.h"
#include "utils.h"

void	init_skydome(t_context *ctx, char *file)
{
	if (file)
		ctx->scene.skydome = load_texture(ctx, file);
}
