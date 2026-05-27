#include "lib_math.h"

t_quat quat_from_dir(t_vec3 dir) {
	const t_vec3 up = { { 0.0f, 1.0f, 0.0f, 0.0f } };

	dir = vec3_normalize(dir);
	float dot = vec3_dot(up, dir);
	if (dot < -0.999f)
		return quat_from_euler(vec3(M_PI, 0.0f, 0.0f));

	t_vec3 cross = vec3_cross(up, dir);
	t_quat q = { //
		.x = cross.x,
		.y = cross.y,
		.z = cross.z,
		.w = 1.0f + dot
	};
	return quat_normalize(q);
}
