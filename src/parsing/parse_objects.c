#include "parsing.h"
#include "objects.h"
#include "libft_str.h"
#include "libft_vector.h"
#include "utils.h"
#include <stdlib.h>

/**
 * Sphere: sp <center> <diameter> <color|mat_id>
 */
t_error	parse_sphere(t_context *ctx, t_parser *p, char **tokens)
{
	t_vec3		center;
	float		diameter;
	uint32_t	mat_id;
	t_material	mat;
	t_error		err;

	if (count_tokens(tokens) < 4)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &center))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tokens[2], &diameter) || diameter <= 0.0f)
		return (PARSE_ERR_RANGE);
	if (!parse_uint(tokens[3], &mat_id))
		return (PARSE_ERR_RANGE);
	err = parse_material_token(p, tokens[3], &mat);
	if (err != PARSE_OK)
		return (err);
	p->has_sphere = true;
	return (init_sphere(ctx, center, diameter, mat_id));
}

/**
 * Plane: pl <point> <normal> <color|mat_id>
 */
t_error	parse_plane(t_context *ctx, t_parser *p, char **tokens)
{
	t_vec3		point;
	t_vec3		normal;
	uint32_t	mat_id;
	t_material	mat;
	t_error		err;

	if (count_tokens(tokens) < 4)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &point) || !parse_vec3(tokens[2], &normal))
		return (PARSE_ERR_INVALID_NUM);
	if (!validate_normalized(normal))
		return (PARSE_ERR_RANGE);
	if (!parse_uint(tokens[3], &mat_id))
		return (PARSE_ERR_RANGE);
	err = parse_material_token(p, tokens[3], &mat);
	if (err != PARSE_OK)
		return (err);
	p->has_plane = true;
	return (init_plane(ctx, point, normal, mat_id));
}

/**
 * Cylinder: cy <center> <axis> <diameter> <height> <color|mat_id>
 */
t_error	parse_cylinder(t_context *ctx, t_parser *p, char **tkns)
{
	t_cylinder	cyl;
	t_material	mat;
	uint32_t	mat_id;
	t_error		err;

	if (count_tokens(tkns) < 6)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tkns[1], &cyl.center) || !parse_vec3(tkns[2], &cyl.axis))
		return (PARSE_ERR_INVALID_NUM);
	if (!validate_normalized(cyl.axis))
		return (PARSE_ERR_RANGE);
	if (!parse_float(tkns[3], &cyl.radius)
		|| !parse_float(tkns[4], &cyl.height))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_uint(tkns[5], &mat_id))
		return (PARSE_ERR_RANGE);
	cyl.radius *= 0.5f;
	if (cyl.radius <= 0.0f || cyl.height <= 0.0f)
		return (PARSE_ERR_RANGE);
	err = parse_material_token(p, tkns[5], &mat);
	if (err != PARSE_OK)
		return (err);
	p->has_cylinder = true;
	return (init_cylinder(ctx, &cyl, mat_id));
}
