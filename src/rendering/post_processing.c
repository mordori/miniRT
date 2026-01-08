#include "rendering.h"

static inline t_vec3	tonemap_aces(t_vec3 color);

t_vec3	post_process(t_vec3 c)
{
	c.r = fmaxf(0.0f, c.r);
	c.g = fmaxf(0.0f, c.g);
	c.b = fmaxf(0.0f, c.b);
	c = tonemap_aces(c);
	c.r = ft_clamp01(c.r);
	c.g = ft_clamp01(c.g);
	c.b = ft_clamp01(c.b);
	return (c);
}

static inline t_vec3	tonemap_aces(t_vec3 c)
{
	t_vec4		val;
	t_vec3		result;
	float		e;

	val = (t_vec4){{2.51f, 0.03f, 2.43f, 0.59f}};
	e = 0.14f;
	result.r = (c.r * (val.r * c.r + val.g)) / (c.r * (val.b * c.r + val.a) + e);
	result.g = (c.g * (val.r * c.g + val.g)) / (c.g * (val.b * c.g + val.a) + e);
	result.b = (c.b * (val.r * c.b + val.g)) / (c.b * (val.b * c.b + val.a) + e);
	return (result);
}
