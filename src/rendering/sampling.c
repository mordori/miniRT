/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sampling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:50:07 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:50:09 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

static inline t_vec3	sample_vndf_hemisphere(t_vec3 wi, t_vec2 uv);

t_vec3	sample_cos_hemisphere(t_vec3 n, t_vec2 uv)
{
	const float		cos_theta = sqrtf(uv.v);
	const float		sin_theta = sqrtf(fmaxf(0.0f, 1.0f - uv.v));

	return (spherical_to_world(n, uv.u, sin_theta, cos_theta));
}

// Shirley's concentric mapping
t_vec2	sample_disk(t_vec2 uv)
{
	const float		u = 2.0f * uv.u - 1.0f;
	const float		v = 2.0f * uv.v - 1.0f;
	t_vec2			phi;
	float			r;

	if (u == 0.0f && v == 0.0f)
		return (vec2_n(0.0f));
	if (u * u > v * v)
	{
		r = u;
		sincosf(M_PI_4 * (v / u), &phi.sin, &phi.cos);
	}
	else
	{
		r = v;
		sincosf(M_PI_2 - M_PI_4 * (u / v), &phi.sin, &phi.cos);
	}
	return (vec2(r * phi.cos, r * phi.sin));
}

t_vec3	sample_cone(t_vec3 vec, float cos_theta_max, t_vec2 uv)
{
	t_vec3		res;
	float		cos_theta;
	float		sin_theta;

	cos_theta = 1.0f + uv.v * (cos_theta_max - 1.0f);
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));
	res = spherical_to_world(vec, uv.u, sin_theta, cos_theta);
	return (res);
}

// Visible GGX Normals, Dupuy & Benyoub
void	sample_ggx_vndf(t_path *path, float alpha, t_vec2 uv)
{
	t_vec3		wi_std;
	t_vec3		wm_std;
	t_vec3		wm;
	t_vec3		t;
	t_vec3		b;

	onb(path->n, &t, &b);
	wi_std = vec3(vec3_dot(path->v, t), vec3_dot(path->v, b), path->ndotv);
	wi_std.z = fmaxf(wi_std.z, G_EPSILON);
	wi_std = vec3_normalize(vec3(wi_std.x * alpha, wi_std.y * alpha, wi_std.z));
	wm_std = sample_vndf_hemisphere(wi_std, uv);
	wm = vec3_normalize(vec3(wm_std.x * alpha, wm_std.y * alpha, wm_std.z));
	path->h = mul_tbn(wm, path->n, t, b);
}

static inline t_vec3	sample_vndf_hemisphere(t_vec3 wi, t_vec2 uv)
{
	t_vec3		h;
	t_vec3		c;
	float		sin_theta;
	float		cos_theta;

	uv.v = fminf(uv.v, 0.9999f);
	cos_theta = (1.0f - uv.v) * (1.0f + wi.z) - wi.z;
	sin_theta = sqrtf(fmaxf(0.0f, (1.0f - cos_theta * cos_theta)));
	c = spherical_to_cartesian(uv.u, sin_theta, cos_theta);
	h = vec3_add(c, wi);
	return (h);
}
