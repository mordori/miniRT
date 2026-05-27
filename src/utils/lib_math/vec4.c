#include "lib_math.h"

t_vec4 vec4(float x, float y, float z, float w) {
	return (t_vec4){ { x, y, z, w } };
}

t_vec4 vec4_n(float n) {
	return (t_vec4){ { n, n, n, n } };
}

t_vec4 vec4_add(t_vec4 a, t_vec4 b) {
	return (t_vec4){ .v = a.v + b.v };
}

t_vec4 vec4_sub(t_vec4 a, t_vec4 b) {
	return (t_vec4){ .v = a.v - b.v };
}

t_vec4 vec4_scale(t_vec4 vec, float s) {
	return (t_vec4){ .v = vec.v * v4sf_n(s) };
}
