#include "utils.h"

// Samples a low discrepency sequence, where points are as evenly spaced as
// possible, using Cranley-Patterson Rotation (random shift modulo).
//
// Pre-calculated texture in /assets/textures/ by Christoph Peters
// https://momentsingraphics.de/BlueNoise.html
float	blue_noise(const t_texture *tex, const t_pixel *pixel, uint32_t dim)
{
	uint32_t	tx;
	uint32_t	ty;
	uint32_t	index;
	uint32_t	seed;

	seed = pixel->frame + ((dim / 4u) * UINT_PRIME);
	tx = (pixel->x + hash_lowerbias32(seed)) & (tex->width - 1u);
	ty = (pixel->y + hash_lowerbias32(seed + 1u)) & (tex->height - 1u);
	index = (ty * tex->width + tx) * 4u;
	return (tex->pixels[index + (dim & 3u)]);
}

float	static_blue_noise(const t_texture *tex, const t_pixel *pixel, uint32_t dim)
{
	uint32_t	tx;
	uint32_t	ty;
	uint32_t	index;
	uint32_t	seed;

	seed = ((dim / 4u) * UINT_PRIME);
	tx = (pixel->x + hash_lowerbias32(seed)) & (tex->width - 1u);
	ty = (pixel->y + hash_lowerbias32(seed + 1u)) & (tex->height - 1u);
	index = (ty * tex->width + tx) * 4u;
	return (tex->pixels[index + (dim & 3u)]);
}

/*
** Samples a 1D low discrepancy sequence (Golden Ratio sequence).
** Uses Cranley-Patterson Rotation by adding an offset and wrapping modulo 1.0
** This gracefully preserves the spatial correlation of Blue Noise on deeper
** bounces without degrading into white noise (which causes clumping).
** https://www.graphics.rwth-aachen.de/media/papers/jgt.pdf
*/
float	r1_sequence(uint32_t n, float offset)
{
	float		res;

	res = offset + (float)n * ONE_MINUS_GR;
	res -= floorf(res);
	return (res);
}

/*
** Samples a 2D low discrepancy sequence (R2 sequence) based on the generalized
** Golden Ratio (plastic constant). By adding a blue noise offset and taking
** the fractional part, we get a perfectly uniform sampling distribution.
*/
t_vec2	r2_sequence(uint32_t n, t_vec2 offset)
{
	t_vec2		res;
	float		u;
	float		v;

	u = offset.u + (float)n * ONE_MINUS_PC;
	v = offset.v + (float)n * ONE_MINUS_PC2;
	res.u = u - floorf(u);
	res.v = v - floorf(v);
	return (res);
}

t_vec2	r4_sequence_d12(uint32_t n, t_vec2 offset)
{
	t_vec2		res;
	float		u;
	float		v;

	u = offset.u + (float)n * 0.1433251161454971f;
	v = offset.v + (float)n * 0.2661081433728707f;
	res.u = u - floorf(u);
	res.v = v - floorf(v);
	return (res);
}
