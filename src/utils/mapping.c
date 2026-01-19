#include "utils.h"

t_vec3	map_spherical(float u, float v)
{
	float		phi;
	float		cos_theta;
	float		sin_theta;

	phi = M_TAU * u;
	cos_theta = 1.0f - 2.0f * v;
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));
	return ((t_vec3){{
		sin_theta * cosf(phi),
		sin_theta * sinf(phi),
		cos_theta
	}});
}
