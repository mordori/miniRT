#include "utils.h"

static inline float	srgb_to_linear(float c);

t_texture	load_texture(t_context *ctx, char *file)
{
	t_texture	texture;

	texture.tex = mlx_load_png(file);
	if (!texture.tex)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	texture.pixels = malloc(sizeof(float) * texture.tex->width * texture.tex->height * 4);
	if (!texture.pixels)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	tex_to_linear(&texture);
	mlx_delete_texture(texture.tex);
	texture.tex = NULL;
	return (texture);
}

void	tex_to_linear(t_texture *texture)
{
	uint32_t	pixels_total;
	uint32_t	i;

	i = 0;
	texture->width = texture->tex->width;
	texture->height = texture->tex->height;
	pixels_total = texture->width * texture->height * 4;
	while (i < pixels_total)
	{
		texture->pixels[i] = srgb_to_linear((float)texture->tex->pixels[i] * INV_255F);
		texture->pixels[i + 1] = srgb_to_linear((float)texture->tex->pixels[i + 1] * INV_255F);
		texture->pixels[i + 2] = srgb_to_linear((float)texture->tex->pixels[i + 2] * INV_255F);
		texture->pixels[i + 3] = srgb_to_linear((float)texture->tex->pixels[i + 3] * INV_255F);
		i += 4;
	}
}

static inline float	srgb_to_linear(float c)
{
	return (powf(c, 2.2f));
}

void	free_texture(t_texture *texture)
{
	if (texture->tex)
		mlx_delete_texture(texture->tex);
	if (texture->pixels)
		free(texture->pixels);
}
