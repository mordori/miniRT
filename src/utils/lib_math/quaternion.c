#include "lib_math.h"

t_quat quat_from_euler(t_vec3 euler) {
	t_vec2 x, y, z;
	euler = vec3_scale(euler, 0.5f);
	sincosf(euler.x, &x.sin, &x.cos);
	sincosf(euler.y, &y.sin, &y.cos);
	sincosf(euler.z, &z.sin, &z.cos);
	return (t_quat){ //
		.w = x.cos * y.cos * z.cos + x.sin * y.sin * z.sin,
		.x = x.sin * y.cos * z.cos - x.cos * y.sin * z.sin,
		.y = x.cos * y.sin * z.cos + x.sin * y.cos * z.sin,
		.z = x.cos * y.cos * z.sin - x.sin * y.sin * z.cos
	};
}

t_quat quat_from_euler_angle(t_vec3 axis, float angle) {
	t_vec2 theta;
	sincosf(angle * 0.5f, &theta.sin, &theta.cos);
	axis = vec3_normalize(axis);
	t_vec3 imag = vec3_scale(axis, theta.sin);
	return (t_quat){ //
		.x = imag.x,
		.y = imag.y,
		.z = imag.z,
		.w = theta.cos
	};
}

t_quat quat_normalize(t_quat q) {
	float len_sq = vec4_dot(q, q);
	if (len_sq > LEN_SQ_EPSILON)
		q = vec4_scale(q, 1.0f / sqrtf(len_sq));
	return q;
}

t_mat4 quat_to_mat4(t_quat q) {
	t_mat4 m = mat4_identity();
	t_vec3 x = vec3(q.x * q.x, q.x * q.y, q.x * q.z);
	t_vec3 yz = vec3(q.y * q.z, q.y * q.y, q.z * q.z);
	t_vec3 w = vec3(q.w * q.x, q.w * q.y, q.w * q.z);
	m.m[0][0] = 1.0f - 2.0f * (yz.y + yz.z);
	m.m[0][1] = 2.0f * (x.y - w.z);
	m.m[0][2] = 2.0f * (x.z + w.y);
	m.m[1][0] = 2.0f * (x.y + w.z);
	m.m[1][1] = 1.0f - 2.0f * (x.x + yz.z);
	m.m[1][2] = 2.0f * (yz.x - w.x);
	m.m[2][0] = 2.0f * (x.z - w.y);
	m.m[2][1] = 2.0f * (yz.x + w.x);
	m.m[2][2] = 1.0f - 2.0f * (x.x + yz.y);
	return m;
}

t_quat quat_mul(t_quat a, t_quat b) {
	return (t_quat){ //
		.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
		.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
		.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
	};
}
