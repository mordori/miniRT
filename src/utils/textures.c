#include "utils.h"

t_texture	*load_texture(t_context *ctx, char *file)
{
	t_texture	*tex;

	tex = mlx_load_png(file);
	if (!tex)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	return (tex);
}

void	free_texture(t_texture *tex)
{
	if (tex)
		mlx_delete_texture(tex);
}
