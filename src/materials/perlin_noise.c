#include "materials.h"
#include <math.h>

/*
** Ken Perlin's permutation table (256 entries, wrapped via & 255).
** The original 512-entry table was this same data repeated twice;
** masking the index with & 255 achieves identical wrapping.
** https://raytracing.github.io/books/RayTracingTheNextWeek.html
*/
static uint8_t	perm(int i)
{
	static const uint8_t	t[256] = {
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
		29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

	return (t[i & 255]);
}

/*
** Quintic fade curve: 6t^5 - 15t^4 + 10t^3
** Provides C2 continuity (second derivative is continuous),
** which eliminates visible grid artifacts in the noise.
*/
static inline float	fade(float t)
{
	return (t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f));
}

/*
** Hash-based gradient selection.
** Uses the low 4 bits of the hash to select one of 12 gradient directions.
** This is Ken Perlin's improved noise gradient function.
*/
static float	grad(int hash, float x, float y, float z)
{
	int	h;
	float	u;
	float	v;

	h = hash & 15;
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
	return (u + v);
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
static float	perlin_lerp(const int *h, t_vec3 f, t_vec3 uvw)
{
	float	x1;
	float	x2;
	float	y1;
	float	y2;

	x1 = ft_lerp(grad(perm(h[0]), f.x, f.y, f.z),
			grad(perm(h[2]), f.x - 1.0f, f.y, f.z), uvw.x);
	x2 = ft_lerp(grad(perm(h[1]), f.x, f.y - 1.0f, f.z),
			grad(perm(h[3]), f.x - 1.0f, f.y - 1.0f, f.z), uvw.x);
	y1 = ft_lerp(x1, x2, uvw.y);
	x1 = ft_lerp(grad(perm(h[0] + 1), f.x, f.y, f.z - 1.0f),
			grad(perm(h[2] + 1), f.x - 1.0f, f.y, f.z - 1.0f), uvw.x);
	x2 = ft_lerp(grad(perm(h[1] + 1), f.x, f.y - 1.0f, f.z - 1.0f),
			grad(perm(h[3] + 1), f.x - 1.0f, f.y - 1.0f, f.z - 1.0f),
			uvw.x);
	y2 = ft_lerp(x1, x2, uvw.y);
	return (ft_lerp(y1, y2, uvw.z));
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
float	perlin_noise(float x, float y, float z)
{
	int		cell[3];
	t_vec3	f;
	t_vec3	uvw;
	int		h[4];

	cell[0] = (int)floorf(x) & 255;
	cell[1] = (int)floorf(y) & 255;
	cell[2] = (int)floorf(z) & 255;
	f = vec3(x - floorf(x), y - floorf(y), z - floorf(z));
	uvw = vec3(fade(f.x), fade(f.y), fade(f.z));
	h[0] = perm(perm(cell[0]) + cell[1]) + cell[2];
	h[1] = perm(perm(cell[0]) + cell[1] + 1) + cell[2];
	h[2] = perm(perm(cell[0] + 1) + cell[1]) + cell[2];
	h[3] = perm(perm(cell[0] + 1) + cell[1] + 1) + cell[2];
	return (perlin_lerp(h, f, uvw));
}
