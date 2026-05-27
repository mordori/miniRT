#include "lib_math.h"

t_vec3 lerp_color(uint32_t c1, uint32_t c2, float t) {
	t_v4sf a = v4sf((c1 >> 24) & 0xFF, (c1 >> 16) & 0xFF, (c1 >> 8) & 0xFF, 255.0f);
	t_v4sf b = v4sf((c2 >> 24) & 0xFF, (c2 >> 16) & 0xFF, (c2 >> 8) & 0xFF, 255.0f);
	return (t_vec3){ .v = ((1.0f - t) * a + t * b) * M_1_255F };
}

uint32_t rgba_to_abgr(uint32_t rgba) {
	t_color abgr = { //
		.r = (rgba & 0xFF) << 24,
		.g = (rgba & 0xFF00) << 8,
		.b = (rgba & 0xFF0000) >> 8,
		.a = (rgba & 0xFF000000) >> 24
	};
	return abgr.full;
}
