#include "rendering.h"
#include "materials.h"
#include "utils.h"

t_vec3	background_color(const t_texture *tex, const t_ray *ray, const float lux, t_vec2 uv_offset)
{
	t_vec2		uv;

	if (!tex->pixels)
		return (vec3_scale(background_gradient((ray->dir.y + 1.0f) * 0.5f), lux * 1.5f));
	uv = spherical_uv(ray->dir);
	uv = vec2_add(uv, uv_offset);
	return (vec3_scale(sample_texture(tex, uv), lux));
}

t_vec3	background_gradient(const float t)
{
	t_vec3		res;

	res = lerp_color(RED, BLUE, t);
	return (res);
}
