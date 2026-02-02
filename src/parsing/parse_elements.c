#include "camera.h"
#include "libft_io.h"
#include "lib_math.h"
#include "libft_str.h"
#include "lights.h"
#include "parsing.h"
#include "utils.h"

// ∗ identifier: A
// ∗ ambient lighting ratio in the range [0.0,1.0]: 0.2
// ∗ R, G, B colors in the range [0-255]: 255, 255, 255
t_parse_error	parse_ambient(t_context *ctx, t_parser *p, char **tokens)
{
	float	ratio;
	t_vec3	color;

	if (count_tokens(tokens) != 3)
		return (PARSE_ERR_MISSING_ARGS);
	// check for duplicate ambient light
	if (p->has_ambient)
		return (PARSE_ERR_DUPLICATE);
	if (!parse_float(tokens[1], &ratio))
		return (PARSE_ERR_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		ratio = ft_clamp(ratio, 0.0f, 1.0f);
	if (!parse_color(tokens[2], &color))
		return (PARSE_ERR_INVALID_NUM);
	ctx->scene.ambient_light.type = LIGHT_AMBIENT;
	ctx->scene.ambient_light.intensity = ratio;
	ctx->scene.ambient_light.color = color;
	p->has_ambient = true;
	return (PARSE_OK);
}

// ∗ identifier: L
// ∗ x, y, z coordinates of the light point: -40.0,50.0,0.0
// ∗ the light brightness ratio in the range [0.0,1.0]: 0.6
// ∗ R, G, B colors in the range [0-255]: 10,0,255
t_parse_error	parse_light(t_context *ctx, t_parser *p, char **tokens)
{
	float	ratio;
	t_vec3	position;
	t_vec3	color;
	t_light	light;

	if (count_tokens(tokens) != 4)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &position))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tokens[2], &ratio))
		return (PARSE_ERR_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		return (PARSE_ERR_RANGE);
	if (!parse_color(tokens[3], &color))
		return (PARSE_ERR_INVALID_NUM);
	light = (t_light){0};
	light.type = LIGHT_POINT;
	light.pos = position;
	light.intensity = ratio;
	light.color = color;
	init_point_light(ctx, &light);
	p->has_light = true;
	return (PARSE_OK);
}

// ∗ identifier: C
// ∗ x, y, z coordinates of the viewpoint: -50.0,0,20
// ∗ 3D normalized orientation vector, in the range [-1,1] for each x, y,z
// ∗ FOV: Horizontal field of view in degrees in the range [0,180]: 70
t_parse_error	parse_camera(t_context *ctx, t_parser *p, char **tokens)
{
	t_vec3	position;
	t_vec3	orientation;
	float	fov;

	if (count_tokens(tokens) != 4)
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
	init_camera(ctx, position, orientation, fov);
	p->has_camera = true;
	return (PARSE_OK);
}

bool	validate_normalized(t_vec3 vec)
{
	float	length;
	float	epsilon;

	epsilon = 1e-6f;
	if (vec.x < -1.0f || vec.x > 1.0f || vec.y < -1.0f || vec.y > 1.0f
		|| vec.z < -1.0f || vec.z > 1.0f)
		return (false);
	length = vec3_length(vec);
	return (fabsf(length - 1.0f) < epsilon);
}

bool	validate_range(float value, float min, float max)
{
	return (value >= min && value <= max);
}
