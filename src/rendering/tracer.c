#include "rendering.h"

t_vec4	trace(t_context *ctx, t_ray *ray)
{
	uint32_t	u;
	uint32_t	v;
	uint32_t	width;
	uint32_t	height;
	uint32_t	idx;

	width = ctx->scene.skydome->width;
	height = ctx->scene.skydome->height;
	if (ray->t == M_INF)
	{
		if (!ctx->scene.skydome)
			return ((t_vec4){{255.999f, 255.999f, 255.999f, 255.999f}});
		u = width * atan2f(ray->dir.z, ray->dir.x) * M_2_PI_INV - 0.5f;
		v = height * acos(ray->dir.y) * M_PI_INV - 0.5f;
		idx = (u + v * width) % (width * height);
		(void)idx;
		// ctx->scene.sky_dome.
	}
	return ((t_vec4){0});
}
