#include "utils.h"
#include "libft_random.h"

// Samples a pre-calculated blue noise texture by Christoph Peters
// https://momentsingraphics.de/BlueNoise.html
float	blue_noise(const t_texture *tex, const t_pixel *pixel, uint32_t dim)
{
	uint32_t	tx;
	uint32_t	ty;
	uint32_t	index;
	uint32_t	seed;

	seed = pixel->frame + (dim * 15485863u);
	tx = (pixel->x + (hash_lowerbias32(seed) % tex->width)) % tex->width;
	ty = (pixel->y + (hash_lowerbias32(seed + 1) % tex->height)) % tex->height;
	index = (ty * tex->width + tx) * 4u;
	return (tex->pixels[index + (dim % 4u)]);
}
