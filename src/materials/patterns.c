#include "lib_math.h"
#include "materials.h"


static t_vec3	pattern_gradient(const t_hit *hit, const t_material *mat);
static t_vec3 pattern_checkerboard2(t_hit *hit, t_material *mat);
// static t_vec3 pattern_checkerboard(t_hit *hit, t_material *mat);

t_vec3	eval_pattern(t_material *mat, t_hit *hit)
{
	if (mat->pattern == PAT_NONE)
		return (mat->albedo);
	if (mat->pattern == PAT_CHECKERBOARD)
		return (pattern_checkerboard2(hit, mat));
	if (mat->pattern == PAT_GRADIENT)
		return (pattern_gradient(hit, mat));
	return (mat->albedo);
}

// static t_vec3 pattern_checkerboard(t_hit *hit, t_material *mat)
// {
// 	float scale;
// 	int x;
// 	int y;
// 	int z;

// 	scale = mat->pattern_scale;
// 	if (scale < 0.001f)
// 		scale = 1.0f;
// 	x = (int)floorf(hit->point.x * scale); // (int) alone is not enough for negative coordinates, floorf works with both positive and negative correctly
// 	y = (int)floorf(hit->point.y * scale);
// 	z = (int)floorf(hit->point.z * scale);
// 	if ((x + y + z) & 1)
// 		return (mat->albedo2);
// 	return (mat->albedo);
// }

#include "immintrin.h" // auto-vectorizes does the same thing as above but in one instruction, and should be faster.. still need more testings though.
static t_vec3 pattern_checkerboard2(t_hit *hit, t_material *mat)
{
    float   scale;
    __m128i cell;
    int     sum;


    scale = mat->pattern_scale;
    if (scale < 0.001f)
        scale = 1.0f;
    // floor(pos * scale) for x, y, z in one instruction
    cell = _mm_cvtps_epi32(_mm_floor_ps(_mm_mul_ps(hit->point.v, _mm_set1_ps(scale))));
    // The SSE extract instructions (_mm_extract_epi32) to pull the result back out cost cycles too, so the overhead nearly cancels the gain.
    sum = _mm_extract_epi32(cell, 0)  // x
        + _mm_extract_epi32(cell, 1)  // y
        + _mm_extract_epi32(cell, 2); // z

    if (sum & 1)
        return (mat->albedo2);
    return (mat->albedo);
}

static t_vec3	pattern_gradient(const t_hit *hit, const t_material *mat)
{
    float t;

    t = hit->uv.v;
    return (vec3_lerp(mat->albedo, mat->albedo2, t));
}
