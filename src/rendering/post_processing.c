#include "rendering.h"
#include "utils.h"
#include "libft_matrix.h"

static inline t_vec3	linear_to_srgb(t_vec3 c);
static inline t_vec3	tonemap_aces(t_vec3 color);
static inline t_vec3	tonemap_aces_fast(t_vec3 color);
static inline float		aces_fast(float x);

t_vec3	post_process(const t_context *ctx, const t_pixel *pixel, t_vec3 c)
{
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure);
	c = tonemap_aces(c);
	c = linear_to_srgb(c);
	c.r += (blue_noise(&ctx->tex_bn, pixel, BN_PP_R) - 0.5f) * INV_255F;
	c.g += (blue_noise(&ctx->tex_bn, pixel, BN_PP_G) - 0.5f) * INV_255F;
	c.b += (blue_noise(&ctx->tex_bn, pixel, BN_PP_B) - 0.5f) * INV_255F;
	return (vec3_clamp01(c));
}

t_vec3	post_process_fast(const t_context *ctx, t_vec3 c)
{
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure * 0.6f);
	c = tonemap_aces_fast(c);
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

static inline t_vec3	tonemap_aces_fast(t_vec3 c)
{
	t_vec3		result;

	result.r = aces_fast(c.r);
	result.g = aces_fast(c.g);
	result.b = aces_fast(c.b);
	return (result);
}

// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
static inline float	aces_fast(float x)
{
	return ((x * (2.51f * x + 0.03f)) / (x * (2.43f * x + 0.59f) + 0.14f));
}

// Reference Rendering Transform and Output Device Transform
static inline t_vec3	rrt_and_odt_fit(t_vec3 vec)
{
	t_vec3	a;
	t_vec3	b;

	a.r = vec.r * (vec.r + 0.0245786f) - 0.000090537f;
	a.g = vec.g * (vec.g + 0.0245786f) - 0.000090537f;
	a.b = vec.b * (vec.b + 0.0245786f) - 0.000090537f;
	b.r = vec.r * (0.983729f * vec.r + 0.4329510f) + 0.238081f;
	b.g = vec.g * (0.983729f * vec.g + 0.4329510f) + 0.238081f;
	b.b = vec.b * (0.983729f * vec.b + 0.4329510f) + 0.238081f;
	return (vec3(a.r / b.r, a.g / b.g, a.b / b.b));
}

// ACES workflow by Stephen Hill
// https://github.com/TheRealMJP/BakingLab/blob/master/BakingLab/ACES.hlsl
static inline t_vec3	tonemap_aces(t_vec3 color)
{
	static const t_mat3	aces_input =
	{{
		{0.59719f, 0.35458f, 0.04823f},
		{0.07600f, 0.90834f, 0.01566f},
		{0.02840f, 0.13383f, 0.83777f}
	}};
	static const t_mat3	aces_output =
	{{
		{1.60475f, -0.53108f, -0.07367f},
		{-0.10208f, 1.10813f, -0.00605f},
		{-0.00327f, -0.07276f, 1.07602f}
	}};

	color = mat3_mul_vec3(aces_input, color);
	color = rrt_and_odt_fit(color);
	color = mat3_mul_vec3(aces_output, color);
	return (vec3_clamp01(color));
}
