#include "camera.h"
#include "libft_io.h"
#include "lib_math.h"
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
		return (PARSE_ERR_MISSING_ARGS);
	if (p->has_ambient)
		return (PARSE_ERR_DUPLICATE);
	if (!parse_float(tokens[1], &ratio))
		return (PARSE_ERR_INVALID_NUM);
	if (!validate_range(ratio, 0.0f, 1.0f))
		return (PARSE_ERR_RANGE);
	if (!parse_color(tokens[2], &color))
		return (PARSE_ERR_INVALID_NUM);
	ctx->scene.ambient_light.type = LIGHT_AMBIENT;
	ctx->scene.ambient_light.intensity = ratio;
	ctx->scene.ambient_light.color = color;
	p->has_ambient = true;
	return (PARSE_OK);
}

/**
 * Point Light: L <position> <intensity> <color> [radius]
 * If radius is provided, configures the light for area light sampling.
 */
t_error	parse_light(t_context *ctx, t_parser *p, char **tokens)
{
	float	ratio;
	float	radius;
	t_vec3	position;
	t_vec3	color;
	t_light	light;
	int		token_count;
	uint32_t	mat_id;

	token_count = count_tokens(tokens);
	// if (token_count < 4 || token_count > 5)
	// 	return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &position))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tokens[2], &ratio))
		return (PARSE_ERR_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		return (PARSE_ERR_RANGE);
	if (!parse_color(tokens[3], &color))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_uint(tokens[5], &mat_id))
		return (PARSE_ERR_RANGE);
	radius = 0.0f;
	if (token_count >= 5 && !parse_float(tokens[4], &radius))
		return (PARSE_ERR_INVALID_NUM);
	light = (t_light){0};
	light.type = LIGHT_POINT;
	light.pos = position;
	light.intensity = ratio;
	light.color = color;
	light.radius = radius;
	light.material_id = mat_id;
	init_point_light(ctx, &light);
	p->has_light = true;
	return (PARSE_OK);
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
		return (PARSE_ERR_MISSING_ARGS);
	if (p->has_camera)
		return (PARSE_ERR_DUPLICATE);
	if (!parse_vec3(tokens[1], &position))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_vec3(tokens[2], &orientation))
		return (PARSE_ERR_INVALID_NUM);
	if (!validate_normalized(orientation))
		return (PARSE_ERR_RANGE);
	if (!parse_float(tokens[3], &fov))
		return (PARSE_ERR_INVALID_NUM);
	if (!validate_range(fov, 0.0f, 180.0f))
		return (PARSE_ERR_RANGE);
	exposure = 0.085f;
	if (token_count == 5 && !parse_float(tokens[4], &exposure))
		return (PARSE_ERR_INVALID_NUM);
	ctx->scene.cam.exposure = exposure;
	init_camera(ctx, position, orientation, fov);
	p->has_camera = true;
	return (PARSE_OK);
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
