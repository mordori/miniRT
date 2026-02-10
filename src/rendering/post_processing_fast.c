#include "rendering.h"

static inline t_vec3	tonemap_aces_fast(t_vec3 color);
static inline float		aces_fast(float x);

t_vec3	post_process_fast(const t_context *ctx, t_vec3 c)
{
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure * 0.6f);
	c = tonemap_aces_fast(c);
	c = vec3_sqrt(c);
	return (c);
}

// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
static inline t_vec3	tonemap_aces_fast(t_vec3 c)
{
	t_vec3		res;
	t_v4sf		a;
	t_v4sf		b;

	a = c.v * (2.51f * c.v + 0.03f);
	b = c.v * (2.43f * c.v + 0.59f) + 0.14f;
	res.v = a / b;
	return (res);
}

static inline float	aces_fast(float x)
{
	return ((x * (2.51f * x + 0.03f)) / (x * (2.43f * x + 0.59f) + 0.14f));
}
