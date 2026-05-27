#include "lib_math.h"

t_vec3 spherical_to_cartesian(float u, float sin_theta, float cos_theta) {
	t_vec2 phi;
	sincosf(M_TAU * u, &phi.sin, &phi.cos);
	return (t_vec3){ //
		.x = sin_theta * phi.cos,
		.y = sin_theta * phi.sin,
		.z = cos_theta
	};
}

t_vec3 spherical_to_world(t_vec3 n, float u, float sin_theta, float cos_theta) {
	t_vec3 local = spherical_to_cartesian(u, sin_theta, cos_theta);
	return tangent_to_world(local, n);
}

t_vec3 tangent_to_world(t_vec3 vec, t_vec3 n) {
	t_vec3 t, b;
	onb(n, &t, &b);
	return mul_tbn(vec, n, t, b);
}

// Pixar Orthonormal Basis, Duff et al.
void onb(t_vec3 n, t_vec3* b1, t_vec3* b2) {
	float sign = copysignf(1.0f, n.z);
	float a = -1.0f / (sign + n.z);
	float b = n.x * n.y * a;
	*b1 = vec3(1.0f + sign * n.x * n.x * a, sign * b, -sign * n.x);
	*b2 = vec3(b, sign + n.y * n.y * a, -n.y);
}

t_vec3 mul_tbn(t_vec3 vec, t_vec3 n, t_vec3 t, t_vec3 b) {
	t_vec3 res = vec3_add(vec3_scale(t, vec.x), vec3_scale(b, vec.y));
	res = vec3_add(res, vec3_scale(n, vec.z));
	return res;
}
