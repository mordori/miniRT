#include "rendering.h"

static inline t_vec3	tonemap_aces(t_vec3 color);
static inline float	aces(float x);

t_vec3	post_process(t_vec3 c)
{
	// static const float	exposure = 0.5f;
	c.r = fmaxf(0.0f, c.r);
	c.g = fmaxf(0.0f, c.g);
	c.b = fmaxf(0.0f, c.b);
	// c = vec3_scale(c, exposure);
	c = tonemap_aces(c);
	c.r = sqrtf(c.r);
	c.g = sqrtf(c.g);
	c.b = sqrtf(c.b);
	c.r = ft_clamp01(c.r);
	c.g = ft_clamp01(c.g);
	c.b = ft_clamp01(c.b);
	return (c);
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
