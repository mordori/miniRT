#include "lib_math.h"

t_mat4 mat4_shear(t_vec3 x, t_vec3 y, t_vec3 z) {
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ 1.0f, x.y, x.z, 0.0f },
		.rows[1] = (t_v4sf){ y.x, 1.0f, y.z, 0.0f },
		.rows[2] = (t_v4sf){ z.x, z.y, 1.0f, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}
