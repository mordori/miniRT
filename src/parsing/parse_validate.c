#include "parsing.h"
#include "materials.h"

bool	validate_normalized(t_vec3 vec)
{
	float	length;

	if (vec.x < -1.0f || vec.x > 1.0f || vec.y < -1.0f || vec.y > 1.0f
		|| vec.z < -1.0f || vec.z > 1.0f)
		return (false);
	length = vec3_length(vec);
	return (fabsf(length - 1.0f) < 1e-6f);
}

bool	validate_range(float value, float min, float max)
{
	return (value >= min && value <= max);
}

/**
 * Resolve material token to both a material struct and a material ID.
 * For mat ID tokens, returns the existing ID.
 * For inline color tokens, registers a new material and returns its ID.
 */
t_error	resolve_material(t_context *ctx, t_parser *p, const char *token,
		uint32_t *out_id)
{
	t_material	mat;
	uint32_t	id;
	t_error		err;

	err = parse_material_token(p, token, &mat);
	if (err != E_OK)
		return (err);
	if (parse_uint((char *)token, &id))
		*out_id = id;
	else
		*out_id = new_material(ctx, &mat);
	return (E_OK);
}
