#include "camera.h"
#include "lib_math.h"
#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"

/**
 * Ambient Light: A <ratio> <color>
 */
t_error	parse_ambient(t_context *ctx, t_parser *p, char **tokens)
{
	float	ratio;
	t_vec3	color;

	if (count_tokens(tokens) != 3)
		return (E_MISSING_ARGS);
	if (p->has_ambient)
		return (E_DUPLICATE);
	if (!parse_float(tokens[1], &ratio))
		return (E_INVALID_NUM);
	if (!validate_range(ratio, 0.0f, 1.0f))
		return (E_RANGE);
	if (!parse_color(tokens[2], &color))
		return (E_INVALID_NUM);
	ctx->scene.ambient_light.type = LIGHT_AMBIENT;
	ctx->scene.ambient_light.intensity = ratio;
	ctx->scene.ambient_light.color = color;
	p->has_ambient = true;
	return (E_OK);
}

/**
 * Point Light: L <position> <intensity> <color> [radius] [mat_id]
 * radius defaults to 0.5 if not specified (area light with soft shadows).
 * mat_id defaults to 0 if not specified.
 */
t_error	parse_light(t_context *ctx, t_parser *p, char **tokens)
{
	float		ratio;
	float		radius;
	t_vec3		position;
	t_vec3		color;
	t_light		light;
	int			token_count;
	uint32_t	mat_id;
	t_material	*mat;

	token_count = count_tokens(tokens);
	if (token_count < 4 || token_count > 6)
		return (E_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &position))
		return (E_INVALID_NUM);
	if (!parse_float(tokens[2], &ratio))
		return (E_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		return (E_RANGE);
	if (!parse_color(tokens[3], &color))
		return (E_INVALID_NUM);
	// radius = 0.5f; // Default radius for area light
	if (token_count >= 5 && !parse_float(tokens[4], &radius))
		return (E_INVALID_NUM);
	if (token_count >= 5 && (radius <= 0.0f))
		return (E_INVALID_NUM);
	mat_id = 0; // Default material ID
	if (token_count >= 6 && !parse_uint(tokens[5], &mat_id))
		return (E_RANGE);
	mat = get_material_by_id(p, mat_id);
	if (!mat)
		return (E_MATERIAL);
	if (!mat->is_emissive)
		return (E_EMISSIVE);
	light = (t_light){0};
	light.type = LIGHT_POINT;
	light.pos = position;
	light.intensity = ratio;
	light.color = color;
	light.radius = radius;
	init_point_light(ctx, &light, mat_id);
	p->has_light = true;
	return (E_OK);
}

/**
 * Camera: C <position> <orientation> <fov> [exposure]
 * Exposure defaults to 0.085f if not specified.
 */
t_error	parse_camera(t_context *ctx, t_parser *p, char **tokens)
{
	t_vec3	position;
	t_vec3	orientation;
	float	fov;
	float	exposure;
	int		token_count;

	token_count = count_tokens(tokens);
	if (token_count < 4 || token_count > 5)
		return (E_MISSING_ARGS);
	if (p->has_camera)
		return (E_DUPLICATE);
	if (!parse_vec3(tokens[1], &position))
		return (E_INVALID_NUM);
	if (!parse_vec3(tokens[2], &orientation))
		return (E_INVALID_NUM);
	if (!validate_normalized(orientation))
		return (E_RANGE);
	if (!parse_float(tokens[3], &fov))
		return (E_INVALID_NUM);
	if (!validate_range(fov, 0.0f, 180.0f))
		return (E_RANGE);
	exposure = 0.0f;
	if (token_count == 5 && !parse_float(tokens[4], &exposure))
		return (E_INVALID_NUM);
	if (!validate_range(exposure, 0.01f, 10.0f))
		return (E_RANGE);
	ctx->scene.cam.exposure = exposure;
	init_camera(ctx, position, orientation, fov);
	p->has_camera = true;
	return (E_OK);
}

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
