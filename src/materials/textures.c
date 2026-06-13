/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "materials.h"
#include "utils.h"

static inline void tex_to_linear(t_texture* texture, bool is_srgb);

static float g_lut[256];

t_texture load_texture(t_context* ctx, char* file, bool is_srgb) {
	printf("\033[1;33mLoading texture:    %s\033[0m\n", file);
	t_texture texture = (t_texture){ 0 };
	texture.tex = mlx_load_png(file);
	if (!texture.tex)
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);

	if (!is_pot(texture.tex->width) || !is_pot(texture.tex->height)) {
		mlx_delete_texture(texture.tex);
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
	}

	size_t size = (sizeof(float) * texture.tex->width * texture.tex->height * 4);
	texture.pixels = a_alloc(64, size);
	if (!texture.pixels) {
		mlx_delete_texture(texture.tex);
		fatal_error(ctx, errors(ERR_TEX), __FILE__, __LINE__);
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
			*dst++ = (((float)(*src++)) * M_1_255f);
		}
	} else {
		while (dst < end)
			*dst++ = (((float)(*src++)) * M_1_255f);
	}
	mlx_delete_texture(texture->tex);
	texture->tex = NULL;
}

void lut_srgb_to_linear(void) {
	static bool init_lut;

	if (!init_lut) {
		size_t i = 0;
		while (i < 256) {
			g_lut[i] = powf((float)i * M_1_255f, 2.2f);
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

/**
 * Get a single texel (pixel) from texture at integer coordinates.
 * Assumes coordinates are valid (within bounds).
 * Pixels stored as RGBA floats, we extract RGB.
 */
static inline t_vec3 get_texel(const float* pixels, uint32_t idx) {
	const float* p = __builtin_assume_aligned(pixels, 64);
	t_vec3 result;
	memcpy(&result, &p[idx], sizeof(t_vec3));
	return result;
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
static inline void compute_bilinear_coords(const t_texture* tex, t_vec2 uv, uint32_t* coords, float* weights) {
	uv.u = uv.u - floorf(uv.u);
	uv.v = uv.v - floorf(uv.v);
	uint32_t mask_x = tex->width - 1;
	uint32_t mask_y = tex->height - 1;
	float px = uv.u * (float)tex->width;
	float py = uv.v * (float)tex->height;
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
 * Uses POT bitwise ops: shift replaces multiply for stride
 *
 * Algorithm:
 *   1. Find 4 surrounding pixels (x0,y0), (x1,y0), (x0,y1), (x1,y1)
 *   2. Compute fractional position (fx, fy) within the pixel cell
 *   3. Lerp horizontally: top = lerp(TL, TR, fx), bottom = lerp(BL, BR, fx)
 *   4. Lerp vertically: result = lerp(top, bottom, fy)
 */
t_vec3 sample_texture(const t_texture* tex, t_vec2 uv) {
	uint32_t coords[4];
	float weights[2];

	compute_bilinear_coords(tex, uv, coords, weights);
	coords[1] *= (tex->width << 2);
	coords[3] *= (tex->width << 2);
	coords[0] <<= 2;
	coords[2] <<= 2;
	t_vec3 top = vec3_lerp(get_texel(tex->pixels, coords[1] + coords[0]), get_texel(tex->pixels, coords[1] + coords[2]), weights[0]);
	t_vec3 bottom = vec3_lerp(get_texel(tex->pixels, coords[3] + coords[0]), get_texel(tex->pixels, coords[3] + coords[2]), weights[0]);
	return vec3_lerp(top, bottom, weights[1]);
}

/**
 * Get surface color from material based on base_color type.
 * Uses texture if available, otherwise returns albedo.
 */
t_vec3 get_surface_color(const t_material* mat, const t_hit* hit) {
	if (mat->is_texture && mat->texture.pixels)
		return sample_texture(&mat->texture, hit->uv);
	return mat->albedo;
}
