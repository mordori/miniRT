/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:45:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:45:31 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "lib_math.h"
#include "libft_io.h"
#include "libft_str.h"
#include "lights.h"
#include "materials.h"
#include "parsing.h"
#include "utils.h"

static t_error	parse_ambient_color2(t_context *ctx, char **tokens)
{
	t_vec3	color;

	if (count_tokens(tokens) == 4)
	{
		if (!parse_color(tokens[3], &color))
			return (E_INVALID_NUM);
		ctx->scene.env.amb_color_2 = color;
	}
	else if (count_tokens(tokens) != 3)
		return (E_ARGS);
	return (E_OK);
}

t_error	parse_ambient(t_context *ctx, t_parser *p, char **tokens)
{
	float	ratio;
	t_vec3	color;
	t_error	err;

	if (p->has_ambient)
		return (E_DUPLICATE);
	err = parse_ambient_color2(ctx, tokens);
	if (err != E_OK)
		return (err);
	if (!parse_float(tokens[1], &ratio))
		return (E_INVALID_NUM);
	if (!validate_range(ratio, 0.0f, 1.0f))
		return (E_RANGE);
	if (!parse_color(tokens[2], &color))
		return (E_INVALID_NUM);
	ctx->scene.env.amb_light.type = LIGHT_AMBIENT;
	ctx->scene.env.amb_light.intensity = ratio;
	ctx->scene.env.amb_light.color = color;
	p->has_ambient = true;
	return (E_OK);
}

static t_error	parse_camera_fields(char **tokens, t_vec3 *pos, t_vec3 *orient,
		float *fov)
{
	if (!parse_vec3(tokens[1], pos))
		return (E_INVALID_NUM);
	if (!parse_vec3(tokens[2], orient))
		return (E_INVALID_NUM);
	if (!validate_normalized(*orient))
		return (E_RANGE);
	if (!parse_float(tokens[3], fov))
		return (E_INVALID_NUM);
	if (!validate_range(*fov, 0.0f, 180.0f))
		return (E_RANGE);
	return (E_OK);
}

t_error	parse_camera(t_context *ctx, t_parser *p, char **tokens)
{
	t_vec3	position;
	t_vec3	orientation;
	float	fov;
	t_error	err;

	if (count_tokens(tokens) != 6)
		return (E_ARGS);
	if (p->has_camera)
		return (E_DUPLICATE);
	err = parse_camera_fields(tokens, &position, &orientation, &fov);
	if (err != E_OK)
		return (err);
	ctx->scene.cam.exposure = 0.0f;
	if (!parse_float(tokens[4], &ctx->scene.cam.exposure))
		return (E_INVALID_NUM);
	if (!validate_range(ctx->scene.cam.exposure, 0.01f, 10.0f))
		return (E_RANGE);
	if (!parse_float(tokens[5], &ctx->scene.cam.focus_dist))
		return (E_INVALID_NUM);
	init_camera(ctx, position, orientation, fov);
	p->has_camera = true;
	return (E_OK);
}
