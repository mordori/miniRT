#include "lib_math.h"

t_vec3	sample_cos_hemisphere(t_vec3 n, t_vec2 uv)
{
	float		cos_theta;
	float		sin_theta;
	t_vec3		res;

	uv.v = fmaxf(uv.v, 0.00001f);
	cos_theta = sqrtf(uv.v);
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - uv.v));
	res = spherical_to_world(n, uv.u, cos_theta, sin_theta);
	return (res);
}

t_vec3	sample_ggx(t_vec3 n, float alpha, t_vec2 uv)
{
	float		cos_theta;
	float		sin_theta;
	t_vec3		res;

	uv.v = fminf(uv.v, 0.99999f);
	uv.v = fmaxf(uv.v, 0.00001f);
	cos_theta = sqrtf((1.0f - uv.v) / (1.0f + (alpha * alpha - 1.0f) * uv.v));
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));
	res = spherical_to_world(n, uv.u, cos_theta, sin_theta);
	return (res);
}

t_vec3	sample_cone(t_vec3 vec, float cos_theta_max, t_vec2 uv)
{
	float		cos_theta;
	float		sin_theta;
	t_vec3		res;

	cos_theta = 1.0f + uv.v * (cos_theta_max - 1.0f);
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));
	res = spherical_to_world(vec, uv.u, cos_theta, sin_theta);
	return (res);
}
