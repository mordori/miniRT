#include "lib_math.h"

t_vec4 vec4_3(t_vec3 vec, float w) {
	t_vec4 res;
	res.v = vec.v;
	res.w = w;
	return res;
}

float vec4_dot(t_vec4 a, t_vec4 b) {
	t_v4sf res = a.v * b.v;
	return res[0] + res[1] + res[2] + res[3];
}
