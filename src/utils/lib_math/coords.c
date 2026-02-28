#include "lib_math.h"

t_vec3	spherical_to_cartesian(float u, float sin_theta, float cos_theta)
{
	t_vec2		phi;
	float		x;
	float		y;
	float		z;

	sincosf(M_TAU * u, &phi.sin, &phi.cos);
	x = sin_theta * phi.cos;
	y = sin_theta * phi.sin;
	z = cos_theta;
	return (vec3(x, y, z));
}

t_vec3	spherical_to_world(t_vec3 n, float u, float sin_theta, float cos_theta)
{
	t_vec3		local;
	t_vec3		world;

	local = spherical_to_cartesian(u, sin_theta, cos_theta);
	world = tangent_to_world(local, n);
	return (world);
}

t_vec3	tangent_to_world(t_vec3 vec, t_vec3 n)
{
	t_vec3		world;
	t_vec3		t;
	t_vec3		b;

	onb(n, &t, &b);
	world = mul_tbn(vec, n, t, b);
	return (world);
}

void	onb(t_vec3 n, t_vec3 *b1, t_vec3 *b2)
{
	float		sign;
	float		a;
	float		b;

	sign = copysignf(1.0f, n.z);
	a = -1.0f / (sign + n.z);
	b = n.x * n.y * a;
	*b1 = vec3(1.0f + sign * n.x * n.x * a, sign * b, -sign * n.x);
	*b2 = vec3(b, sign + n.y * n.y * a, -n.y);
}

t_vec3	mul_tbn(t_vec3 vec, t_vec3 n, t_vec3 t, t_vec3 b)
{
	t_vec3		res;

	res = vec3_add(vec3_scale(t, vec.x), vec3_scale(b, vec.y));
	res = vec3_add(res, vec3_scale(n, vec.z));
	return (res);
}
