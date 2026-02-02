#include "utils.h"

t_vec3	map_spherical(const float u, const float v)
{
	t_vec3		res;
	float		cos_theta;
	float		sin_theta;
	t_vec2		phi;

	cos_theta = 1.0f - 2.0f * v;
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));
	sincosf(M_TAU * u, &phi.sin, &phi.cos);
	res = vec3(sin_theta * phi.cos, sin_theta * phi.sin, cos_theta);
	return (res);
}
