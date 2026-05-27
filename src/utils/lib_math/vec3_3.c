#include "lib_math.h"

t_vec3 vec3_mul(t_vec3 a, t_vec3 b) {
	return (t_vec3){ .v = a.v * b.v };
}

t_vec3 vec3_reflect(t_vec3 vec, t_vec3 n) {
	return vec3_sub(vec, vec3_scale(n, 2 * vec3_dot(vec, n)));
}

t_vec3 vec3_unit_random(uint32_t* seed) {
	while (true) {
		t_vec3 res = vec3(randomfn11(seed), randomfn11(seed), randomfn11(seed));
		float len_sq = vec3_dot(res, res);
		if (len_sq <= 1.0f && len_sq >= LEN_SQ_EPSILON)
			return vec3_div(res, sqrtf(len_sq));
	}
}

t_vec3 vec3_min(t_vec3 vec, float min) {
	t_v4sf limit = v4sf(min, min, min, M_INF);
	return (t_vec3){ .v = _mm_min_ps(vec.v, limit) };
}

t_vec3 vec3_max(t_vec3 vec, float max) {
	t_v4sf limit = v4sf(max, max, max, -M_INF);
	return (t_vec3){ .v = _mm_max_ps(vec.v, limit) };
}
