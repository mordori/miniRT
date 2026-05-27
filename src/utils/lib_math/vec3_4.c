#include "lib_math.h"

t_vec3 vec3_sqrt(t_vec3 vec) {
	return (t_vec3){ .v = _mm_sqrt_ps(vec.v) };
}

t_vec3 vec3_clamp(t_vec3 vec, float min, float max) {
	t_vec3 res;
	res.v = _mm_max_ps(vec.v, v4sf_n(min));
	res.v = _mm_min_ps(res.v, v4sf_n(max));
	return res;
}

t_vec3 vec3_clamp01(t_vec3 vec) {
	return vec3_clamp(vec, 0.0f, 1.0f);
}

t_vec3 vec3_clamp_mag(t_vec3 vec, float max) {
	float len_sq = vec3_dot(vec, vec);
	if (len_sq > max * max) {
		float scale = max / sqrtf(len_sq);
		return vec3_scale(vec, scale);
	}
	return vec;
}

t_vec3 vec3_add_n(t_vec3 vec, float n) {
	return (t_vec3){ .v = vec.v + v4sf_n(n) };
}
