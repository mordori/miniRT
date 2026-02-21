#include "utils.h"

static inline void	srgb_to_linear(float *lut);
static inline void	tex_data_to_linear(t_texture *texture);
static inline void	tex_srgb_to_linear(t_texture *texture, float *lut);

t_texture	load_texture(char *file, bool is_srgb)
{
	t_texture		texture;
	size_t			size;
	static float	lut[256];

	srgb_to_linear(lut);
	printf("Loading texture:    %s\n", file);
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
		tex_srgb_to_linear(&texture, lut);
	else
		tex_data_to_linear(&texture);
	mlx_delete_texture(texture.tex);
	texture.tex = NULL;
	return (texture);
}

static inline void	tex_data_to_linear(t_texture *texture)
{
	float		*dst;
	float		*end;
	uint8_t		*src;

	dst = texture->pixels;
	src = texture->tex->pixels;
	end = dst + (texture->width * texture->height * 4);
	while (dst < end)
	{
		*dst++ = ((float)(*src++) * M_1_255F);
		*dst++ = ((float)(*src++) * M_1_255F);
		*dst++ = ((float)(*src++) * M_1_255F);
		*dst++ = ((float)(*src++) * M_1_255F);
	}
}

static inline void	tex_srgb_to_linear(t_texture *texture, float *lut)
{
	float		*dst;
	float		*end;
	uint8_t		*src;

	dst = texture->pixels;
	src = texture->tex->pixels;
	end = dst + (texture->width * texture->height * 4);
	while (dst < end)
	{
		*dst++ = lut[*src++];
		*dst++ = lut[*src++];
		*dst++ = lut[*src++];
		*dst++ = ((float)(*src++) * M_1_255F);
	}
}

static inline void	srgb_to_linear(float *lut)
{
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
}

void	free_texture(t_texture *texture)
{
	if (texture->tex)
		mlx_delete_texture(texture->tex);
	if (texture->pixels)
		free(texture->pixels);
}
