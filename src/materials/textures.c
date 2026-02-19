#include "materials.h"
#include "utils.h"

/**
 * Linear interpolation between two colors.
 * Returns a + (b - a) * t = a * (1 - t) + b * t
 */
static inline t_vec3	vlerp(t_vec3 a, t_vec3 b, float t)
{
	return (vec3_add(a, vec3_scale(vec3_sub(b, a), t)));
}

/**
 * Get a single texel (pixel) from texture at integer coordinates.
 * Assumes coordinates are valid (within bounds).
 * Pixels stored as RGBA floats, we extract RGB.
 */
static inline t_vec3	get_texel(const float *pixels, uint32_t idx)
{
	const float	*p;
	t_vec3		result;

	p = (const float *)__builtin_assume_aligned(pixels, 64);
	memcpy(&result, &p[idx], sizeof(t_vec3));
	return (result);
}

/**
 * Compute bilinear sample coordinates and weights using POT bitwise tricks.
 *
 * Since textures are guaranteed power-of-two, (width - 1) forms a bitmask:
 *   e.g. width=256 → mask=0xFF, so (x & mask) wraps to [0, 255]
 *   This replaces ft_uint_min/clamp with a single AND — ~1 cycle vs ~20.
 *
 * coords[0..3] = x0, y0, x1, y1 (pixel indices, wrapped via bitmask)
 * weights[0..1] = fx, fy (fractional sub-pixel position for lerp blending)
 */
static inline void	compute_bilinear_coords(const t_texture *tex, t_vec2 uv,
	uint32_t *coords, float *weights)
{
	float		px;
	float		py;
	uint32_t	mask_x;
	uint32_t	mask_y;

	mask_x = tex->width - 1;		/* POT bitmask: 256 → 0xFF */
	mask_y = tex->height - 1;
	px = uv.u * mask_x;
	py = uv.v * mask_y;
	coords[0] = (uint32_t)px & mask_x;
	coords[1] = (uint32_t)py & mask_y;
	coords[2] = (coords[0] + 1) & mask_x;
	coords[3] = (coords[1] + 1) & mask_y;
	weights[0] = px - (uint32_t)px;
	weights[1] = py - (uint32_t)py;
}

/**
 * Sample a texture at given UV coordinates using bilinear filtering.
 * Blends 4 neighboring pixels weighted by distance for smooth results.
 * Uses POT bitwise ops: shift replaces multiply for stride (width << 2 == width * 4).
 *
 * Algorithm:
 *   1. Find 4 surrounding pixels (x0,y0), (x1,y0), (x0,y1), (x1,y1)
 *   2. Compute fractional position (fx, fy) within the pixel cell
 *   3. Lerp horizontally: top = lerp(TL, TR, fx), bottom = lerp(BL, BR, fx)
 *   4. Lerp vertically: result = lerp(top, bottom, fy)
 */
t_vec3	sample_texture(const t_texture *tex, t_vec2 uv)
{
	uint32_t	coords[4];
	float		weights[2];
	uint32_t	row[2];
	uint32_t	col[2];
	t_vec3		top;
	t_vec3		bottom;

	if (__builtin_expect(!tex->pixels, 0))
		return (vec3_n(1.0f));
	compute_bilinear_coords(tex, uv, coords, weights);
	row[0] = coords[1] * (tex->width << 2);
	row[1] = coords[3] * (tex->width << 2);
	col[0] = coords[0] << 2;
	col[1] = coords[2] << 2;
	// add some preprocessing later
	top = vlerp(
			get_texel(tex->pixels, row[0] + col[0]),
			get_texel(tex->pixels, row[0] + col[1]),
			weights[0]);
	bottom = vlerp(
			get_texel(tex->pixels, row[1] + col[0]),
			get_texel(tex->pixels, row[1] + col[1]),
			weights[0]);
	return (vlerp(top, bottom, weights[1]));
}

/**
 * Get surface color from material based on base_color type.
 * Uses texture if available, otherwise returns albedo.
 */
t_vec3	get_surface_color(const t_material *mat, const t_hit *hit)
{
	if (mat->base_color == BASE_TEXTURE && mat->texture.pixels)
		return (vec3_mul(mat->albedo, sample_texture(&mat->texture, hit->uv)));
	if (mat->base_color == BASE_PATTERN)
		return (eval_pattern((t_material *)mat, (t_hit *)hit));
	return (mat->albedo);
}
