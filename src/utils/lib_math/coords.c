#include "lib_math.h"

t_vec3	spherical_coords(float u, float sin_theta, float z)
{
	t_vec2		phi;
	float		x;
	float		y;

	sincosf(M_TAU * u, &phi.sin, &phi.cos);
	x = sin_theta * phi.cos;
	y = sin_theta * phi.sin;
	return (vec3(x, y, z));
}

t_vec3	spherical_to_world(t_vec3 n, float u, float sin_theta, float z)
{
	t_vec3		local;

	local = spherical_coords(u, sin_theta, z);
	return (tangent_to_world(local, n));
}

t_vec3	tangent_to_world(t_vec3 vec, t_vec3 n)
{
	t_vec3		res;
	t_vec3		t;
	t_vec3		b;

	orthonormal_basis(n, &t, &b);
	res = vec3_add(vec3_scale(t, vec.x), vec3_scale(b, vec.y));
	res = vec3_add(res, vec3_scale(n, vec.z));
	return (res);
}

void	orthonormal_basis(t_vec3 n, t_vec3 *t, t_vec3 *b)
{
	t_vec3		temp;

	if (fabsf(n.x) > 0.9f)
		temp = vec3(0.0f, 1.0f, 0.0f);
	else
		temp = vec3(1.0f, 0.0f, 0.0f);
	*b = vec3_normalize(vec3_cross(n, temp));
	*t = vec3_cross(n, *b);
}
