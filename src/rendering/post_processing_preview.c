#include "rendering.h"

static inline t_vec3	tonemap_aces_preview(t_vec3 color);

t_vec3	post_process_preview(const t_context *ctx, t_vec3 c)
{
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure * 0.565f);
	c = tonemap_aces_preview(c);
	c = vec3_sqrt(c);
	return (c);
}

// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
static inline t_vec3	tonemap_aces_preview(t_vec3 c)
{
	t_vec3		res;
	t_v4sf		a;
	t_v4sf		b;

	a = c.v * (2.51f * c.v + 0.03f);
	b = c.v * (2.43f * c.v + 0.59f) + 0.14f;
	res.v = a / b;
	return (res);
}
