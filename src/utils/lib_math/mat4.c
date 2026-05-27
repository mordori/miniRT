#include "lib_math.h"

t_mat4 mat4_identity(void) {
	static const t_v4sf r0 = { 1.0f, 0.0f, 0.0f, 0.0f };
	static const t_v4sf r1 = { 0.0f, 1.0f, 0.0f, 0.0f };
	static const t_v4sf r2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	static const t_v4sf r3 = { 0.0f, 0.0f, 0.0f, 1.0f };

	return (t_mat4){ //
		.rows[0] = r0,
		.rows[1] = r1,
		.rows[2] = r2,
		.rows[3] = r3
	};
}

t_mat4 mat4_transpose(const t_mat4* src) {
	__m128 c0 = (__m128)src->rows[0];
	__m128 c1 = (__m128)src->rows[1];
	__m128 c2 = (__m128)src->rows[2];
	__m128 c3 = (__m128)src->rows[3];
	_MM_TRANSPOSE4_PS(c0, c1, c2, c3);

	return (t_mat4){ //
		.rows[0] = (t_v4sf)c0,
		.rows[1] = (t_v4sf)c1,
		.rows[2] = (t_v4sf)c2,
		.rows[3] = (t_v4sf)c3
	};
}

t_mat4 mat4_mul(const t_mat4* a, const t_mat4* b) {
	t_mat4 res;
	int i = 0;
	while (i < 4) {
		t_v4sf a_row = a->rows[i];
		res.rows[i] = v4sf_n(a_row[0]) * b->rows[0];
		res.rows[i] += v4sf_n(a_row[1]) * b->rows[1];
		res.rows[i] += v4sf_n(a_row[2]) * b->rows[2];
		res.rows[i] += v4sf_n(a_row[3]) * b->rows[3];
		++i;
	}
	return res;
}

t_vec4 mat4_mul_vec4(const t_mat4* src, t_vec4 vec) {
	__m128 c0 = (__m128)src->rows[0];
	__m128 c1 = (__m128)src->rows[1];
	__m128 c2 = (__m128)src->rows[2];
	__m128 c3 = (__m128)src->rows[3];
	_MM_TRANSPOSE4_PS(c0, c1, c2, c3);

	// clang-format off
	t_vec4 res;
	res.v = (t_v4sf)c0 * v4sf_n(vec.x) +
			(t_v4sf)c1 * v4sf_n(vec.y) +
			(t_v4sf)c2 * v4sf_n(vec.z) +
			(t_v4sf)c3 * v4sf_n(vec.w);
	// clang-format on
	return (res);
}
