#include "lib_math.h"

t_mat4 mat4_translate(t_vec3 t) {
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ 1.0f, 0.0f, 0.0f, t.x },
		.rows[1] = (t_v4sf){ 0.0f, 1.0f, 0.0f, t.y },
		.rows[2] = (t_v4sf){ 0.0f, 0.0f, 1.0f, t.z },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

t_mat4 mat4_scale(t_vec3 s) {
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ s.x, 0.0f, 0.0f, 0.0f },
		.rows[1] = (t_v4sf){ 0.0f, s.y, 0.0f, 0.0f },
		.rows[2] = (t_v4sf){ 0.0f, 0.0f, s.z, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

t_mat4 mat4_rot_x(float rad) {
	float c, s;
	sincosf(rad, &s, &c);
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ 1.0f, 0.0f, 0.0f, 0.0f },
		.rows[1] = (t_v4sf){ 0.0f, c, -s, 0.0f },
		.rows[2] = (t_v4sf){ 0.0f, s, c, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

t_mat4 mat4_rot_y(float rad) {
	float c, s;
	sincosf(rad, &s, &c);
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ c, 0.0f, s, 0.0f },
		.rows[1] = (t_v4sf){ 0.0f, 1.0f, 0.0f, 0.0f },
		.rows[2] = (t_v4sf){ -s, 0.0f, c, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}

t_mat4 mat4_rot_z(float rad) {
	float c, s;
	sincosf(rad, &s, &c);
	return (t_mat4){ //
		.rows[0] = (t_v4sf){ c, -s, 0.0f, 0.0f },
		.rows[1] = (t_v4sf){ s, c, 0.0f, 0.0f },
		.rows[2] = (t_v4sf){ 0.0f, 0.0f, 1.0f, 0.0f },
		.rows[3] = (t_v4sf){ 0.0f, 0.0f, 0.0f, 1.0f }
	};
}
