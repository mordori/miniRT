#include "lib_math.h"

t_mat4 mat4_rot(t_vec3 rot) {
	t_vec2 x, y, z;
	sincosf(rot.x, &x.sin, &x.cos);
	sincosf(rot.y, &y.sin, &y.cos);
	sincosf(rot.z, &z.sin, &z.cos);
	return (t_mat4){ //
		.rows[0] = v4sf(y.cos * z.cos, z.cos * x.sin * y.sin - x.cos * z.sin, x.cos * z.cos * y.sin + x.sin * z.sin, 0.0f),
		.rows[1] = v4sf(y.cos * z.sin, x.cos * z.cos + x.sin * y.sin * z.sin, -z.cos * x.sin + x.cos * y.sin * z.sin, 0.0f),
		.rows[2] = v4sf(-y.sin, y.cos * x.sin, x.cos * y.cos, 0.0f),
		.rows[3] = v4sf(0.0f, 0.0f, 0.0f, 1.0f)
	};
}

t_vec3 mat4_mul_vec3(const t_mat4* src, t_vec3 vec) {
	t_vec4 temp = { .v = vec.v };
	return (t_vec3){ .v = mat4_mul_vec4(src, temp).v };
}
