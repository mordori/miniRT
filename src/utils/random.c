#include "lib_math.h"
#include "utils.h"

void random_uv(const t_context* ctx, t_path* path, t_pixel* pixel, t_bn_channel c) {
	uint32_t dim_u = c + (path->bounce * ctx->bn_stride);
	uint32_t dim_v = c + 1 + (path->bounce * ctx->bn_stride);

	if (path->bounce == 0) {
		path->uv = (t_vec2){ //
			.u = blue_noise(&ctx->tex_bn, pixel, dim_u),
			.v = blue_noise(&ctx->tex_bn, pixel, dim_v)
		};
	} else {
		t_vec2 offset = { //
			.u = static_blue_noise(&ctx->tex_bn, pixel, dim_u),
			.v = static_blue_noise(&ctx->tex_bn, pixel, dim_v)
		};
		path->uv = r2_sequence(pixel->frame + (path->bounce * FP_PRIME), offset);
	}
}

// Pre-calculated texture in /assets/textures/ by Christoph Peters
// https://momentsingraphics.de/BlueNoise.html
float blue_noise(const t_texture* tex, const t_pixel* pixel, uint32_t dim) {
	uint32_t seed = pixel->frame + ((dim / 4u) * UINT_PRIME);
	uint32_t tx = (pixel->x + hash_lowerbias32(seed)) & (tex->width - 1u);
	uint32_t ty = (pixel->y + hash_lowerbias32(seed + 1u)) & (tex->height - 1u);
	uint32_t index = (ty * tex->width + tx) * 4u;
	return tex->pixels[index + (dim & 3u)];
}

float static_blue_noise(const t_texture* tex, const t_pixel* pixel, uint32_t dim) {
	uint32_t seed = ((dim / 4u) * UINT_PRIME);
	uint32_t tx = (pixel->x + hash_lowerbias32(seed)) & (tex->width - 1u);
	uint32_t ty = (pixel->y + hash_lowerbias32(seed + 1u)) & (tex->height - 1u);
	uint32_t index = (ty * tex->width + tx) * 4u;
	return tex->pixels[index + (dim & 3u)];
}

/*
** Samples a 1D low discrepancy sequence (Golden Ratio sequence).
** Uses Cranley-Patterson Rotation by adding an offset and wrapping modulo 1.0
** This gracefully preserves the spatial correlation of Blue Noise on deeper
** bounces without degrading into white noise (which causes clumping).
** https://www.graphics.rwth-aachen.de/media/papers/jgt.pdf
*/
float r1_sequence(uint32_t n, float offset) {
	float res = offset + (float)n * ONE_MINUS_GR;
	res -= floorf(res);
	return res;
}

/*
** Samples a 2D low discrepancy sequence (R2 sequence) based on the generalized
** Golden Ratio (plastic constant). By adding a blue noise offset and taking
** the fractional part, we get a perfectly uniform sampling distribution.
*/
t_vec2 r2_sequence(uint32_t n, t_vec2 offset) {
	float u = offset.u + (float)n * ONE_MINUS_PC;
	float v = offset.v + (float)n * ONE_MINUS_PC2;
	return (t_vec2){ //
		.u = u - floorf(u),
		.v = v - floorf(v)
	};
}

t_vec2 r4_sequence_d12(uint32_t n, t_vec2 offset) {
	float u = offset.u + (float)n * 0.1433251161454971f;
	float v = offset.v + (float)n * 0.2661081433728707f;
	return (t_vec2){ //
		.u = u - floorf(u),
		.v = v - floorf(v)
	};
}
