#include "utils.h"

/**
 * Convert UV coordinates to a point on the unit sphere.
 * Inverse of spherical_uv().
 */
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

/**
 * Compute spherical UV coordinates from a unit direction vector.
 * Uses equirectangular projection (same mapping as planetary textures).
 * U = longitude [0, 1], V = latitude [0, 1] (north pole = 0, south = 1)
 * Inverse of map_spherical().
 */
t_vec2	spherical_uv(t_vec3 dir)
{
	t_vec2	uv;

	uv.u = (atan2f(dir.z, dir.x) + (float)M_PI) * M_1_2PI; // Map from [-π, π] to [0, 1]
	uv.v = acosf(clampf(dir.y, -1.0f, 1.0f)) * M_1_PI; // Map from [0, π] to [0, 1]
	return (uv);
}
