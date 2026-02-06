#include "materials.h"
#include "utils.h"

/**
 * Sample a texture at given UV coordinates.
 * UV should be in range [0,1]. Returns RGB color.
 */
t_vec3	sample_texture(const t_texture *tex, t_vec2 uv)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	i;
	const float	*pixels;
	t_vec3		result;

	if (!tex || !tex->pixels)
		return (vec3_n(1.0f)); // White color if texture is missing
	uv.u = ft_clamp01(uv.u);
	uv.v = ft_clamp01(uv.v);
	x = (uint32_t)(uv.u * (tex->width - 1)); // tex coordinates are [0,1], so scale to pixel indices
	y = (uint32_t)(uv.v * (tex->height - 1));
	i = (y * tex->width + x) * 4; // 4 floats per pixel (RGBA)
	pixels = (const float *)__builtin_assume_aligned(tex->pixels, 64); 
	memcpy(&result, &pixels[i], sizeof(t_vec3));
	return (result);
}

/**
 * Get surface color from material based on base_color type.
 * Uses texture if available, otherwise returns albedo.
 */
t_vec3	get_surface_color(const t_material *mat, const t_hit *hit)
{
	if (mat->base_color == BASE_TEXTURE && mat->texture.pixels)
		return (sample_texture(&mat->texture, hit->uv));
	return (mat->albedo);
}
