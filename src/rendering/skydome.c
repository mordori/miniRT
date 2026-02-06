#include "rendering.h"
#include "materials.h"
#include "utils.h"

t_vec3	background_color(const t_texture *tex, const t_ray *ray, const float lux)
{
	t_vec2	uv;
	t_vec3	result;

	if (!tex || !tex->pixels)
		return (vec3_scale(background_gradient((ray->dir.y + 1.0f) * 0.5f), lux));
	uv = spherical_uv(ray->dir);
	result = sample_texture(tex, uv);
	return (vec3_scale(result, lux));
}

// Todo: smoother with floats
t_vec3	background_gradient(const float t)
{
	t_vec3		result;
	uint32_t	c;

	c = lerp_color(RED, BLUE, t);
	result.r = (float)(((c >> 24) & 0xFF) * INV_255F);
	result.g = (float)(((c >> 16) & 0xFF) * INV_255F);
	result.b = (float)(((c >> 8) & 0xFF) * INV_255F);
	return (result);
}

void	init_skydome(t_context *ctx, char *file)
{
	if (file)
		ctx->scene.skydome = load_texture(ctx, file, true);
}
