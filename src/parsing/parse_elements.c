#include "parsing.h"
#include "libft_str.h"
#include "libft_io.h"
#include "libft_math.h"

// ∗ identifier: A
// ∗ ambient lighting ratio in the range [0.0,1.0]: 0.2
// ∗ R, G, B colors in the range [0-255]: 255, 255, 255
t_parse_error parse_ambient(t_context *ctx, t_parser *p, char **tokens)
{
	float ratio;
	t_color color;

	if (count_tokens(tokens) != 3)
		return (PARSE_ERR_MISSING_ARGS);
	// check for duplicate ambient light
	// if (p->has_ambient)
	// 	return (PARSE_ERR_DUPLICATE);
	if (!parse_float(tokens[1], &ratio))
		return (PARSE_ERR_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		ratio = ft_clamp(ratio, 0.0f, 1.0f); // clamp to valid range or could return error
	if (!parse_color(tokens[2], &color))
		return (PARSE_ERR_INVALID_NUM);
	// might need to normalize color here
	ctx->scene.ambient_light.type = LIGHT_AMBIENT;
	ctx->scene.ambient_light.intensity = ratio;
	ctx->scene.ambient_light.color = (t_vec4){{color.r, color.g, color.b, 1.0f}};
	p->has_ambient = true;
	return (PARSE_OK);
}

// ∗ identifier: L
// ∗ x, y, z coordinates of the light point: -40.0,50.0,0.0
// ∗ the light brightness ratio in the range [0.0,1.0]: 0.6
// ∗ R, G, B colors in the range [0-255]: 10,0,255
t_parse_error parse_light(t_context *ctx, t_parser *p, char **tokens)
{
	float ratio;
	t_vec3 position;
	t_color color;
	t_light light;

	if (count_tokens(tokens) != 4)
		return (PARSE_ERR_MISSING_ARGS);
	if (!parse_vec3(tokens[1], &position))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_float(tokens[2], &ratio))
		return (PARSE_ERR_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		ratio = ft_clamp(ratio, 0.0f, 1.0f); // clamp to valid range or could return error
	if (!parse_color(tokens[3], &color)) // might need to normalize color here
		return (PARSE_ERR_INVALID_NUM);
	light = (t_light){0};
	light.type = LIGHT_POINT;
	light.transform.pos = position;
	light.intensity = ratio;
	light.color = (t_vec4){{color.r, color.g, color.b, 1.0f}};
	ctx->scene.directional_light = light; // temporary, should add to lights vector
	p->has_light = true;
	return (PARSE_OK);
}

// ∗ identifier: C
// ∗ x, y, z coordinates of the viewpoint: -50.0,0,20
// ∗ 3D normalized orientation vector, in the range [-1,1] for each x, y, z axis:
// 0.0,0.0,1.0
// ∗ FOV: Horizontal field of view in degrees in the range [0,180]: 70
t_parse_error parse_camera(t_context *ctx, t_parser *p, char **tokens)
{
	t_vec3 position;
	t_vec3 orientation;
	float fov;

	if (count_tokens(tokens) != 4)
		return(PARSE_ERR_MISSING_ARGS);
	// check for duplicate camera
	if (p->has_camera)
		return (PARSE_ERR_DUPLICATE);
	if (!parse_vec3(tokens[1], &position))
		return (PARSE_ERR_INVALID_NUM);
	if (!parse_vec3(tokens[2], &orientation))
		return (PARSE_ERR_INVALID_NUM);
	// validate orientation vector range normalization
	if (!parse_float(tokens[3], &fov))
		return (PARSE_ERR_INVALID_NUM);
	if (fov < 0.0f || fov > 180.0f)
		fov = ft_clamp(fov, 0.0f, 180.0f); // clamp to valid range or could return error
	// set camera parameters
	ctx->scene.cam.transform.pos = position;
	ctx->scene.cam.target.pos = vec3_add(position, orientation);
	ctx->scene.cam.fov = fov;
	p->has_camera = true;
	return (PARSE_OK);
}