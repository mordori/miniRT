#include "utils.h"

static inline float	srgb_to_linear(uint8_t c);
static inline void	tex_data_to_linear(t_texture *texture);
static inline void	tex_srgb_to_linear(t_texture *texture);

t_texture	load_texture(char *file, bool is_srgb)
{
	t_texture	texture;
	size_t		size;

	texture = (t_texture){0};
	texture.tex = mlx_load_png(file);
	if (!texture.tex)
		return (texture);
	if (!ft_is_pot(texture.tex->width) || !ft_is_pot(texture.tex->height))
	{
		mlx_delete_texture(texture.tex);
		return ((t_texture){0});
	}
	size = (sizeof(float) * texture.tex->width * texture.tex->height * 4);
	texture.pixels = a_alloc(64, size);
	if (!texture.pixels)
	{
		mlx_delete_texture(texture.tex);
		return ((t_texture){0});
	}
	texture.width = texture.tex->width;
	texture.height = texture.tex->height;
	if (is_srgb)
		tex_srgb_to_linear(&texture);
	else
		tex_data_to_linear(&texture);
	mlx_delete_texture(texture.tex);
	texture.tex = NULL;
	return (texture);
}

static inline void	tex_data_to_linear(t_texture *texture)
{
	uint32_t	pixels_total;
	uint32_t	i;
	float		*dst;
	uint8_t		*src;

	i = 0;
	dst = texture->pixels;
	src = texture->tex->pixels;
	pixels_total = texture->width * texture->height * 4;
	while (i < pixels_total)
	{
		dst[i] = ((float)src[i] * M_1_255F);
		dst[i + 1] = ((float)src[i + 1] * M_1_255F);
		dst[i + 2] = ((float)src[i + 2] * M_1_255F);
		dst[i + 3] = ((float)src[i + 3] * M_1_255F);
		i += 4;
	}
}

static inline void	tex_srgb_to_linear(t_texture *texture)
{
	uint32_t	pixels_total;
	uint32_t	i;
	float		*dst;
	uint8_t		*src;

	i = 0;
	dst = texture->pixels;
	src = texture->tex->pixels;
	pixels_total = texture->width * texture->height * 4;
	while (i < pixels_total)
	{
		dst[i] = srgb_to_linear(src[i]);
		dst[i + 1] = srgb_to_linear(src[i + 1]);
		dst[i + 2] = srgb_to_linear(src[i + 2]);
		dst[i + 3] = ((float)src[i + 3] * M_1_255F);
		i += 4;
	}
}

static inline float	srgb_to_linear(uint8_t c)
{
	static float	lut[256];
	static bool		init_lut;
	int				i;

	if (!init_lut)
	{
		i = 0;
		while (i < 256)
		{
			lut[i] = powf((float)i * M_1_255F, 2.2f);
			++i;
		}
		init_lut = true;
	}
	return (lut[c]);
}

void	free_texture(t_texture *texture)
{
	if (texture->tex)
		mlx_delete_texture(texture->tex);
	if (texture->pixels)
		free(texture->pixels);
}
