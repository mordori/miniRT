#include "utils.h"

// Samples a low discrepency sequence, where points are as evenly spaced as
// possible, using Cranley-Patterson Rotation (random shift modulo).
//
// Pre-calculated texture in /assets/textures/ by Christoph Peters
// https://momentsingraphics.de/BlueNoise.html
float	blue_noise(const t_texture *tex, const t_pixel *pixel, const uint32_t dim)
{
	uint32_t	tx;
	uint32_t	ty;
	uint32_t	index;
	uint32_t	seed;

	seed = pixel->frame + (dim * 15485863u);
	tx = (pixel->x + hash_lowerbias32(seed)) & (tex->width - 1);
	ty = (pixel->y + hash_lowerbias32(seed + 1)) & (tex->height - 1);
	index = (ty * tex->width + tx) * 4u;
	return (tex->pixels[index + (dim & 3u)]);
}
