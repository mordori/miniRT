#include "rendering.h"
#include "utils.h"

static inline t_vec3 linear_to_srgb(t_vec3 c);
static inline t_vec3 tonemap_aces(t_vec3 color);
static inline t_vec3 tonemap_aces_preview(t_vec3 color);

t_vec3 post_process(const t_context* ctx, const t_pixel* pixel, t_vec3 c) {
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure);
	c = tonemap_aces(c);
	c = linear_to_srgb(c);
	c.r += (blue_noise(&ctx->tex_bn, pixel, BN_PP_R) - 0.5f) * M_1_255f;
	c.g += (blue_noise(&ctx->tex_bn, pixel, BN_PP_G) - 0.5f) * M_1_255f;
	c.b += (blue_noise(&ctx->tex_bn, pixel, BN_PP_B) - 0.5f) * M_1_255f;
	return c;
}

static inline t_vec3 linear_to_srgb(t_vec3 c) {
	return vec3_powf(c, M_1_2P2f);
}

static inline t_vec3 rrt_and_odt_fit(t_vec3 vec) {
	t_v4sf a = vec.v * (vec.v + 0.0245786f) - 0.000090537f;
	t_v4sf b = vec.v * (0.983729f * vec.v + 0.4329510f) + 0.238081f;
	return (t_vec3){ .v = a / b };
}

// ACES workflow by Stephen Hill
static inline t_vec3 tonemap_aces(t_vec3 color) {
	static const t_mat4 aces_input = { { { //
		{ 0.59719f, 0.35458f, 0.04823f },
		{ 0.07600f, 0.90834f, 0.01566f },
		{ 0.02840f, 0.13383f, 0.83777f },
		{ 0.0f, 0.0f, 0.0f } } } };
	static const t_mat4 aces_output = { { { //
		{ 1.60475f, -0.53108f, -0.07367f },
		{ -0.10208f, 1.10813f, -0.00605f },
		{ -0.00327f, -0.07276f, 1.07602f },
		{ 0.0f, 0.0f, 0.0f } } } };

	color = mat4_mul_vec3(&aces_input, color);
	color = rrt_and_odt_fit(color);
	color = mat4_mul_vec3(&aces_output, color);
	return vec3_max(color, 0.0f);
}

t_vec3 post_process_preview(const t_context* ctx, t_vec3 c) {
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure * 0.565f);
	c = tonemap_aces_preview(c);
	c = vec3_sqrt(c);
	return c;
}

// ACES filmic curve by Krzysztof Narkowicz
static inline t_vec3 tonemap_aces_preview(t_vec3 c) {
	t_v4sf a = c.v * (2.51f * c.v + 0.03f);
	t_v4sf b = c.v * (2.43f * c.v + 0.59f) + 0.14f;
	return (t_vec3){ .v = a / b };
}
