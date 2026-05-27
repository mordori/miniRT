#include "lib_math.h"

bool mat4_inverse(const t_mat4* src, t_mat4* out) {
	float det = mat4_det(src);
	if (fabsf(det) < M_EPSILON)
		return false;

	float inv_det = 1.0f / det;
	int row = 0;
	while (row < 4) {
		int col = 0;
		while (col < 4) {
			float cofactor = mat4_cofactor(src, row, col);
			out->m[col][row] = cofactor * inv_det;
			++col;
		}
		++row;
	}
	return true;
}

t_mat3 mat4_sub(const t_mat4* src, int skip_r, int skip_c) {
	t_mat3 res;
	int row = 0;
	int r = 0;
	while (row < 4) {
		if (row == skip_r && ++row)
			continue;
		int c = 0;
		int col = 0;
		while (col < 4) {
			if (col == skip_c && ++col)
				continue;
			res.m[r][c++] = src->m[row][col++];
		}
		++r;
		++row;
	}
	return res;
}

float mat4_cofactor(const t_mat4* src, int row, int col) {
	t_mat3 sub = mat4_sub(src, row, col);
	float det = mat3_det(&sub);
	if ((row + col) & 1)
		return -det;
	else
		return det;
}

float mat4_det(const t_mat4* src) {
	float res = 0.0f;
	int i = 0;
	while (i < 4) {
		res += src->m[0][i] * mat4_cofactor(src, 0, i);
		++i;
	}
	return res;
}

float mat3_det(const t_mat3* src) {
	float res = src->m[0][0] * (src->m[1][1] * src->m[2][2] - src->m[2][1] * src->m[1][2]);
	res -= src->m[0][1] * (src->m[1][0] * src->m[2][2] - src->m[2][0] * src->m[1][2]);
	res += src->m[0][2] * (src->m[1][0] * src->m[2][1] - src->m[2][0] * src->m[1][1]);
	return res;
}
