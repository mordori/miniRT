#include <stddef.h>

#include "utils.h"

static inline void tex_to_linear(t_texture* texture, bool is_srgb);

static float g_lut[256];

t_texture load_texture(char* file, bool is_srgb) {
	printf("Loading texture:    %s\n", file);
	t_texture texture = (t_texture){ 0 };
	texture.tex = mlx_load_png(file);
	if (!texture.tex)
		return (t_texture){ 0 };

	if (!ft_is_pot(texture.tex->width) || !ft_is_pot(texture.tex->height)) {
		mlx_delete_texture(texture.tex);
		return (t_texture){ 0 };
	}

	size_t size = (sizeof(float) * texture.tex->width * texture.tex->height * 4);
	texture.pixels = a_alloc(64, size);
	if (!texture.pixels) {
		mlx_delete_texture(texture.tex);
		return (t_texture){ 0 };
	}

	texture.width = texture.tex->width;
	texture.height = texture.tex->height;
	tex_to_linear(&texture, is_srgb);
	return texture;
}

static inline void tex_to_linear(t_texture* texture, bool is_srgb) {
	uint8_t* src = texture->tex->pixels;
	float* dst = texture->pixels;
	float* end = dst + (texture->width * texture->height * 4);
	if (is_srgb) {
		while (dst < end) {
			*dst++ = g_lut[*src++];
			*dst++ = g_lut[*src++];
			*dst++ = g_lut[*src++];
			*dst++ = (((float)(*src++)) * M_1_255F);
		}
	} else {
		while (dst < end)
			*dst++ = (((float)(*src++)) * M_1_255F);
	}
	mlx_delete_texture(texture->tex);
	texture->tex = NULL;
}

void lut_srgb_to_linear(void) {
	static bool init_lut;

	if (!init_lut) {
		size_t i = 0;
		while (i < 256) {
			g_lut[i] = powf((float)i * M_1_255F, 2.2f);
			++i;
		}
		init_lut = true;
	}
}

void free_texture(t_texture* texture) {
	if (texture->tex)
		mlx_delete_texture(texture->tex);
	if (texture->pixels)
		free(texture->pixels);
}
