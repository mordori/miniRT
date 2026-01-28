#include "rendering.h"
#include "utils.h"

static inline t_vec3	tonemap_aces(t_vec3 color);
static inline t_vec3	linear_to_srgb(t_vec3 c);
static inline float		aces(float x);

t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c)
{
	c = vec3_max(c, 0.0f);
	if (ctx->renderer.mode != RENDER_EDIT)
		c = vec3_scale(c, ctx->scene.cam.exposure);
	c = tonemap_aces(c);
	if (ctx->renderer.mode == RENDER_REFINE)
	{
		c = linear_to_srgb(c);
		c.r += (blue_noise(&ctx->tex_bn, pixel, BN_PP_R) - 0.5f) * INV_255F;
		c.g += (blue_noise(&ctx->tex_bn, pixel, BN_PP_G) - 0.5f) * INV_255F;
		c.b += (blue_noise(&ctx->tex_bn, pixel, BN_PP_B) - 0.5f) * INV_255F;
	}
	else
		c = vec3_sqrt(c);
	return (vec3_clamp01(c));
}

static inline t_vec3	linear_to_srgb(t_vec3 c)
{
	t_vec3		result;

	result.r = powf(c.r, INV_2_2F);
	result.g = powf(c.g, INV_2_2F);
	result.b = powf(c.b, INV_2_2F);
	return (result);
}

static inline t_vec3	tonemap_aces(t_vec3 c)
{
	t_vec3		result;

	result.r = aces(c.r);
	result.g = aces(c.g);
	result.b = aces(c.b);
	return (result);
}

// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
static inline float	aces(float x)
{
	return ((x * (2.51f * x + 0.03f)) / (x * (2.43f * x + 0.59f) + 0.14f));
}
