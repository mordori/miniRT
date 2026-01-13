#include "parsing.h"
#include "objects.h"
#include "libft_str.h"
#include "libft_vector.h"
#include "utils.h"
#include <stdlib.h>

// Sphere:
// sp 0.0,0.0,20.6 12.6 10,0,255
// ∗ identifier: sp
// ∗ x, y, z coordinates of the sphere center: 0.0,0.0,20.6
// ∗ the sphere diameter: 12.6
// ∗ R,G,B colors in the range [0-255]: 10, 0, 255
t_parse_error parse_sphere(t_context *ctx, char **tokens)
{
	t_vec3	center;
	float	diameter;
	t_vec3	color;
	t_material	mat;

	if (count_tokens(tokens) != 4)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &center))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tokens[2], &diameter))
		return (PARSE_ERR_INVALID_NUM);
	if (diameter <= 0.0f)
		return (PARSE_ERR_RANGE);
	if (!parse_color(tokens[3], &color))
		return (PARSE_ERR_INVALID_NUM);
	mat = (t_material){0};
	mat.color = (t_vec4){{color.r * INV_255F, color.g * INV_255F, color.b * INV_255F, 1.0f}};
	mat.base_color = BASE_COLOR;
	return (init_sphere(ctx, center, diameter, &mat));
}

t_parse_error	parse_plane(t_context *ctx, char **tokens)
{
	t_vec3		point;
	t_vec3		normal;
	t_color		color;
	t_material	mat;

	if (count_tokens(tokens) < 4)
		return(PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &point) || !parse_vec3(tokens[2], &normal))
		return(PARSE_ERR_INVALID_NUM);
	if (!validate_normalized(normal))
		return(PARSE_ERR_RANGE);
	if (!parse_color(tokens[3], &color))
		return(PARSE_ERR_INVALID_NUM);
	mat = (t_material){0};
	mat.color = (t_vec4){{color.r * INV_255F, color.g * INV_255F, color.b * INV_255F, 1.0f}};
	mat.base_color = BASE_COLOR;
	return(init_plane(ctx, point, normal, &mat));
}
