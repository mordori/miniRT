/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "materials.h"

/*
** Ken Perlin's permutation table (256 entries, wrapped via & 255).
** The original 512-entry table was this same data repeated twice;
** masking the index with & 255 achieves identical wrapping.
** https://raytracing.github.io/books/RayTracingTheNextWeek.html
*/
static uint8_t perm(int i) {
	// clang-format off
	static const uint8_t t[256] = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7,
		225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190,
		6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
		35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136,
		171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146,
		158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
		55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80,
		73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116,
		188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
		250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207,
		206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170,
		213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
		43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224,
		232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238,
		210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
		107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115,
		121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67,
		29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};
	// clang-format on
	return t[i & 255];
}

/*
** Quintic fade curve: 6t^5 - 15t^4 + 10t^3
** Provides C2 continuity (second derivative is continuous),
** which eliminates visible grid artifacts in the noise.
*/
static inline float fade(float t) {
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

/*
** Hash-based gradient selection.
** Uses the low 4 bits of the hash to select one of 12 gradient directions.
** This is Ken Perlin's improved noise gradient function.
*/
static float grad(int hash, float x, float y, float z) {
	float u;
	float v;

	int h = hash & 15;
	if (h < 8)
		u = x;
	else
		u = y;

	if (h < 4)
		v = y;
	else if (h == 12 || h == 14)
		v = x;
	else
		v = z;

	if (h & 1)
		u = -u;
	if (h & 2)
		v = -v;

	return u + v;
}

/*
** Trilinear interpolation helper for Perlin noise.
** Computes the weighted blend of the 8 gradient contributions at
** the corners of the unit cube containing the sample point.
**
** @param h   - corner hash indices [aa, ab, ba, bb]
** @param f   - fractional position within unit cube (xf, yf, zf)
** @param uvw - fade-curve weights (u, v, w)
** @return    - interpolated noise value
*/
static float perlin_lerp(const int* h, t_vec3 f, t_vec3 uvw) {
	float x1 = ft_lerp_fast(grad(perm(h[0]), f.x, f.y, f.z), grad(perm(h[2]), f.x - 1.0f, f.y, f.z), uvw.x);
	float x2 = ft_lerp_fast(grad(perm(h[1]), f.x, f.y - 1.0f, f.z), grad(perm(h[3]), f.x - 1.0f, f.y - 1.0f, f.z), uvw.x);
	float y1 = ft_lerp_fast(x1, x2, uvw.y);
	x1 = ft_lerp_fast(grad(perm(h[0] + 1), f.x, f.y, f.z - 1.0f), grad(perm(h[2] + 1), f.x - 1.0f, f.y, f.z - 1.0f), uvw.x);
	x2 = ft_lerp_fast(grad(perm(h[1] + 1), f.x, f.y - 1.0f, f.z - 1.0f), grad(perm(h[3] + 1), f.x - 1.0f, f.y - 1.0f, f.z - 1.0f), uvw.x);
	float y2 = ft_lerp_fast(x1, x2, uvw.y);
	return ft_lerp_fast(y1, y2, uvw.z);
}

/*
** 3D Perlin noise.
** Returns a value in approximately [-1, 1].
**
** Algorithm:
**   1. Find the unit cube containing the point
**   2. Find the relative position within the cube
**   3. Apply fade curve to each coordinate
**   4. Hash the 8 cube corners using the permutation table
**   5. Blend the 8 gradient contributions via trilinear interpolation
*/
float perlin_noise(float x, float y, float z) {
	int cell[3];
	int h[4];

	cell[0] = (int)floorf(x) & 255;
	cell[1] = (int)floorf(y) & 255;
	cell[2] = (int)floorf(z) & 255;
	t_vec3 f = vec3(x - floorf(x), y - floorf(y), z - floorf(z));
	t_vec3 uvw = vec3(fade(f.x), fade(f.y), fade(f.z));
	h[0] = perm(perm(cell[0]) + cell[1]) + cell[2];
	h[1] = perm(perm(cell[0]) + cell[1] + 1) + cell[2];
	h[2] = perm(perm(cell[0] + 1) + cell[1]) + cell[2];
	h[3] = perm(perm(cell[0] + 1) + cell[1] + 1) + cell[2];
	return perlin_lerp(h, f, uvw);
}

/*
** Per-object noise offset. Hashes the world position with the canonical
** GLSL sin/fract trick: sinf maps the dot product to [-1, 1], then the
** large multiplier (43758.5453, tuned for that range) scrambles the
** mantissa bits into a uniform [0, 2pi) scalar.
** Golden-ratio multiples {s, s*phi, s*(phi+1)} give three independent
** per-axis offsets so each object samples a distinct noise region.
*/
static inline t_vec3 object_seed(const t_object* obj) {
	float s = sinf(obj->transform.pos.x * 12.9898f + obj->transform.pos.y * 78.233f + obj->transform.pos.z * 37.719f) * 43758.5453f;
	s = (s - floorf(s)) * M_TAUf;
	return (t_vec3){ { s, s * 1.6180339f, s * 2.6180339f } };
}

/*
** 1/f fractal noise: 8 octaves, each doubling frequency and halving
** amplitude. fabsf() folds smooth zero-crossings into sharp cusps,
** creating the jagged ridges used for marble veins and rock.
** Output is always positive, typically [0, ~2] when octaves align.
*/
float turbulence(t_vec3 p) {
	float accum = 0.0f;
	float weight = 1.0f;
	int i = 0;
	while (i < 8) {
		accum += fabsf(perlin_noise(p.x, p.y, p.z)) * weight;
		weight *= 0.5f;
		p = vec3_scale(p, 2.0f);
		++i;
	}
	return accum;
}

/*
** Marble veins: sin(p.z + 10 * turbulence(p)) blended over two colors.
** p.z alone gives flat parallel bands; the x10 turbulence distortion
** kinks and folds them into irregular veins via its sharp fabsf cusps.
** object_seed() shifts all three noise axes with golden-ratio offsets
** so each object samples a distinct region of the noise field.
*/
t_vec3 pattern_perlin_marble(t_hit* hit, const t_material* mat) {
	float scale = mat->pattern_scale;
	t_vec3 p = mat4_mul_vec3(&hit->obj->transform.world_to_object, hit->point);
	p = vec3_mul(p, hit->obj->transform.scale);
	p = vec3_scale(p, scale);
	p = vec3_add(p, object_seed(hit->obj));
	float t = 0.7f * (1.0f + sinf(p.z + 10.0f * turbulence(p)));
	return vec3_lerp(mat->albedo, mat->albedo2, t);
}

/*
** Wood grain: concentric cylindrical rings (dist from Y-axis) gently
** distorted by smooth perlin_noise. Raw perlin_noise is used instead
** of turbulence because wood rings undulate smoothly — turbulence's
** fabsf cusps would make them look cracked rather than organic.
** object_seed() offsets the ring centre, as if cut from a different
** part of the trunk.
*/
t_vec3 pattern_perlin_wood(t_hit* hit, const t_material* mat) {
	float scale = mat->pattern_scale;
	t_vec3 p = mat4_mul_vec3(&hit->obj->transform.world_to_object, hit->point);
	p = vec3_mul(p, hit->obj->transform.scale);
	p = vec3_scale(p, scale);
	p = vec3_add(p, object_seed(hit->obj));
	float dist = sqrtf(p.x * p.x + p.z * p.z);
	float grain = sinf(scale * dist + 4.0f * perlin_noise(p.x, p.y, p.z));
	grain = 0.5f * (grain + 1.0f);
	return vec3_lerp(mat->albedo, mat->albedo2, grain);
}

/*
** Raw turbulence blend: maps turbulence output directly to two colors.
** No sin() wrapper — the fractal noise drives the blend, giving chaotic
** organic shapes suited for rock, clouds, or alien surfaces.
** Clamped to [0, 1] since multiple aligned octaves can exceed 1.0.
*/
t_vec3 pattern_perlin_turb(t_hit* hit, const t_material* mat) {
	float scale = mat->pattern_scale;
	t_vec3 p = mat4_mul_vec3(&hit->obj->transform.world_to_object, hit->point);
	p = vec3_mul(p, hit->obj->transform.scale);
	p = vec3_scale(p, scale);
	p = vec3_add(p, object_seed(hit->obj));
	float t = turbulence(p);
	t = clampf01(t);
	return vec3_lerp(mat->albedo, mat->albedo2, t);
}
