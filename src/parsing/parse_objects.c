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
	t_error		err;

	if (count_tokens(tokens) < 4)
		return (E_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &center))
		return (E_INVALID_NUM);
	if (!parse_float(tokens[2], &diameter) || diameter <= 0.0f)
		return (E_RANGE);
	err = resolve_material(ctx, p, tokens[3], &mat_id);
	if (err != E_OK)
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
	t_error		err;

	if (count_tokens(tokens) < 4)
		return (E_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &point) || !parse_vec3(tokens[2], &normal))
		return (E_INVALID_NUM);
	if (!validate_normalized(normal))
		return (E_RANGE);
	err = resolve_material(ctx, p, tokens[3], &mat_id);
	if (err != E_OK)
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
	uint32_t	mat_id;
	t_error		err;

	if (count_tokens(tkns) < 6)
		return (E_MISSING_ARGS);
	if (!parse_vec3(tkns[1], &cyl.center) || !parse_vec3(tkns[2], &cyl.axis))
		return (E_INVALID_NUM);
	if (!validate_normalized(cyl.axis))
		return (E_RANGE);
	if (!parse_float(tkns[3], &cyl.radius)
		|| !parse_float(tkns[4], &cyl.height))
		return (E_INVALID_NUM);
	cyl.radius *= 0.5f;
	if (cyl.radius <= 0.0f || cyl.height <= 0.0f)
		return (E_RANGE);
	err = resolve_material(ctx, p, tkns[5], &mat_id);
	if (err != E_OK)
		return (err);
	p->has_cylinder = true;
	return (init_cylinder(ctx, &cyl, mat_id));
}

/**
 * Cone: co <apex> <axis> <angle_deg> <height> <color|mat_id>
 */
t_error	parse_cone(t_context *ctx, t_parser *p, char **tkns)
{
	t_cone		cone;
	uint32_t	mat_id;
	float		angle_deg;
	t_error		err;

	if (count_tokens(tkns) < 6)
		return (E_MISSING_ARGS);
	if (!parse_vec3(tkns[1], &cone.apex) || !parse_vec3(tkns[2], &cone.axis))
		return (E_INVALID_NUM);
	if (!validate_normalized(cone.axis))
		return (E_RANGE);
	if (!parse_float(tkns[3], &angle_deg) || !parse_float(tkns[4], &cone.height))
		return (E_INVALID_NUM);
	if (angle_deg <= 0.0f || angle_deg >= 90.0f || cone.height <= 0.0f)
		return (E_RANGE);
	cone.angle = angle_deg * (M_PI / 180.0f);
	err = resolve_material(ctx, p, tkns[5], &mat_id);
	if (err != E_OK)
		return  (err);
	p->has_cone = true;
	return (init_cone(ctx, &cone, mat_id));
}
