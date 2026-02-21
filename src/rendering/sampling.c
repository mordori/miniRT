#include "rendering.h"

static inline t_vec3	sample_vndf_hemisphere(t_vec3 wi, t_vec2 uv);

t_vec3	sample_cos_hemisphere(t_vec3 n, t_vec2 uv)
{
	t_vec3		res;
	float		cos_theta;
	float		sin_theta;

	cos_theta = sqrtf(uv.v);
	sin_theta = sqrtf(fmaxf(0.0f, 1.0f - uv.v));
	res = spherical_to_world(n, uv.u, sin_theta, cos_theta);
	return (res);
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

void	sample_ggx_vndf(t_path *path, float alpha, t_vec2 uv)
{
	t_vec3		wi_std;
	t_vec3		wm_std;
	t_vec3		wm;
	t_vec3		t;
	t_vec3		b;

	orthonormal_basis(path->n, &t, &b);
	wi_std = vec3(vec3_dot(path->v, t), vec3_dot(path->v, b), vec3_dot(path->v, path->n));
	wi_std = vec3_normalize(vec3(wi_std.x * alpha, wi_std.y * alpha, wi_std.z));
	wm_std = sample_vndf_hemisphere(wi_std, uv);
	wm = vec3_normalize(vec3(wm_std.x * alpha, wm_std.y * alpha, wm_std.z));
	path->h = vec3_add(vec3_scale(t, wm.x), vec3_scale(b, wm.y));
	path->h = vec3_add(path->h, vec3_scale(path->n, wm.z));
}

static inline t_vec3	sample_vndf_hemisphere(t_vec3 wi, t_vec2 uv)
{
	t_vec3		h;
	t_vec3		c;
	float		sin_theta;
	float		cos_theta;

	cos_theta = (1.0f - uv.v) * (1.0f + wi.z) - wi.z;
	sin_theta = sqrtf(fmaxf(0.0f, (1.0f - cos_theta * cos_theta)));
	c = spherical_coords(uv.u, sin_theta, cos_theta);
	h = vec3_add(c, wi);
	return (h);
}
