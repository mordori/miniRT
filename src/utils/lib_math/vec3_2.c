#include "lib_math.h"

t_vec3 vec3_cross(t_vec3 a, t_vec3 b) {
	t_v4sf yzx = _mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3, 0, 2, 1)) * _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3, 1, 0, 2));
	t_v4sf zxy = _mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3, 1, 0, 2)) * _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3, 0, 2, 1));

	return (t_vec3){ .v = yzx - zxy };
}

float vec3_dot(t_vec3 a, t_vec3 b) {
	t_v4sf res = a.v * b.v;
	return res[0] + res[1] + res[2];
}

t_vec3 vec3_div(t_vec3 vec, float den) {
	if (fabsf(den) < LEN_EPSILON)
		return (t_vec3){ 0 };

	return vec3_scale(vec, 1.0f / den);
}

t_vec3 vec3_normalize(t_vec3 vec) {
	float len_sq = vec3_dot(vec, vec);
	if (len_sq < LEN_SQ_EPSILON)
		return (t_vec3){ 0 };

	float len_recip = 1.0f / sqrtf(len_sq);
	return (vec3_scale(vec, len_recip));
}

float vec3_length(t_vec3 vec) {
	return (sqrtf(vec3_dot(vec, vec)));
}
