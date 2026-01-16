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
t_parse_error parse_sphere(t_context *ctx, t_parser *parser, char **tokens)
{
	t_vec3	center;
	float	diameter;
	t_vec3	color;
	t_material	mat;

	if (!ctx || !parser || !tokens)
		return (PARSE_ERR_MALLOC);
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
	mat.albedo = color;
	mat.base_color = BASE_COLOR;
	parser->has_sphere = true;
	return (init_sphere(ctx, center, diameter, &mat));
}

// Plane:
// ∗ identifier: pl
// ∗ x, y, z coordinates of a point in the plane: 0.0,0.0,-10.0
// ∗ 3D normalized normal vector, in the range [-1,1] for each x, y, z axis
// ∗ R,G,B colors in the range [0-255]: 0,0,225
t_parse_error	parse_plane(t_context *ctx, t_parser *parser, char **tokens)
{
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_material	mat;

	if (!ctx || !parser || !tokens)
		return (PARSE_ERR_MALLOC);
	if (count_tokens(tokens) < 4)
		return(PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &point) || !parse_vec3(tokens[2], &normal))
		return(PARSE_ERR_INVALID_NUM);
	if (!validate_normalized(normal))
		return(PARSE_ERR_RANGE);
	if (!parse_color(tokens[3], &color))
		return(PARSE_ERR_INVALID_NUM);
	mat = (t_material){0};
	mat.albedo = color;
	mat.base_color = BASE_COLOR;
	parser->has_plane = true;
	return(init_plane(ctx, point, normal, &mat));
}

// Cylinder:
// ∗ identifier: cy
// ∗ x, y, z coordinates of the center of the cylinder: 50.0,0.0,20.6
// ∗ 3D normalized vector of axis of cylinder, in the range [-1,1] for each x,y,z axis
// ∗ the cylinder diameter: 14.2
// ∗ the cylinder height: 21.42
// ∗ R, G, B colors in the range [0,255]: 10, 0, 255
