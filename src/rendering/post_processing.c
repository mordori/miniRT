#include "rendering.h"
#include "utils.h"

static inline t_vec3	tonemap_aces(t_vec3 color);
static inline t_vec3	linear_to_srgb(t_vec3 c);
static inline float	aces(float x);

t_vec3	post_process_fast(const t_context *ctx, t_vec3 c)
{
	c.r = fmaxf(0.0f, c.r);
	c.g = fmaxf(0.0f, c.g);
	c.b = fmaxf(0.0f, c.b);
	c = vec3_scale(c, ctx->scene.cam.exposure);
	c = tonemap_aces(c);
	c.r = sqrtf(c.r);
	c.g = sqrtf(c.g);
	c.b = sqrtf(c.b);
	c.r = ft_clamp01(c.r);
	c.g = ft_clamp01(c.g);
	c.b = ft_clamp01(c.b);
	return (c);
}

t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c)
{
	c.r = fmaxf(0.0f, c.r);
	c.g = fmaxf(0.0f, c.g);
	c.b = fmaxf(0.0f, c.b);
	c = vec3_scale(c, ctx->scene.cam.exposure);
	c = tonemap_aces(c);
	c = linear_to_srgb(c);
	c.r += (blue_noise(&ctx->blue_noise, pixel, 0) - 0.5f) * INV_255F;
	c.g += (blue_noise(&ctx->blue_noise, pixel, 1) - 0.5f) * INV_255F;
	c.b += (blue_noise(&ctx->blue_noise, pixel, 2) - 0.5f) * INV_255F;
	c.r = ft_clamp01(c.r);
	c.g = ft_clamp01(c.g);
	c.b = ft_clamp01(c.b);
	return (c);
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

static inline float	aces(float x)
{
	return ((x * (2.51f * x + 0.03f)) / (x * (2.43f * x + 0.59f) + 0.14f));
}
