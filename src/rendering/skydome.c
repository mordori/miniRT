#include "rendering.h"
#include "utils.h"

t_vec3	background_color(const t_texture *tex, const t_ray *ray, const float lux)
{
	t_float2	uv;
	t_int2		xy;
	uint32_t	i;
	t_vec3		res;
	const float	*pixels;

	if (!tex || !tex->pixels)
		return (vec3_scale(background_gradient((ray->dir.y + 1.0f) * 0.5f), lux));
	uv.u = ft_clamp01((atan2f(ray->dir.z, ray->dir.x) + M_PI) * M_1_2PI);
	uv.v = ft_clamp01(acosf(ray->dir.y) * M_1_PI);
	xy.x = (uint32_t)(uv.u * (tex->width - 1));
	xy.y = (uint32_t)(uv.v * (tex->height - 1));
	i = (xy.y * tex->width + xy.x) * 4;
	pixels = (const float *)__builtin_assume_aligned(tex->pixels, 64);
	memcpy(&res, &pixels[i], sizeof(t_vec3));
	return (vec3_scale(res, lux));
}

t_vec3	background_gradient(const float t)
{
	t_vec3		res;

	res = lerp_color(RED, BLUE, t);
	return (res);
}

void	init_skydome(t_context *ctx, char *file)
{
	if (file)
		ctx->scene.skydome = load_texture(ctx, file, true);
}
