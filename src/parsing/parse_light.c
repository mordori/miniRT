/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:45:40 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:45:44 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "materials.h"
#include "lights.h"

static t_error	parse_light_core(char **tokens, t_light *light, int tc)
{
	t_vec3	position;
	t_vec3	color;
	float	ratio;

	if (!parse_vec3(tokens[1], &position))
		return (E_INVALID_NUM);
	if (!parse_float(tokens[2], &ratio))
		return (E_INVALID_NUM);
	if (ratio < 0.0f || ratio > 1.0f)
		return (E_RANGE);
	if (!parse_color(tokens[3], &color))
		return (E_INVALID_NUM);
	if (tc >= 5 && !parse_float(tokens[4], &light->radius))
		return (E_INVALID_NUM);
	if (tc >= 5 && (light->radius <= 0.0f))
		return (E_INVALID_NUM);
	light->type = LIGHT_POINT;
	light->pos = position;
	light->intensity = ratio;
	light->color = color;
	return (E_OK);
}

static uint32_t	default_emissive_mat(t_context *ctx, t_light *light)
{
	t_material	mat;

	mat = (t_material){0};
	mat.albedo = light->color;
	mat.emission = vec3_scale(light->color, light->intensity);
	mat.is_emissive = true;
	return (new_material(ctx, &mat));
}

static t_error	parse_light_mat(t_parser *p, char **tokens, uint32_t *mat_id)
{
	t_material	*mat;

	if (!parse_uint(tokens[5], mat_id))
		return (E_RANGE);
	mat = get_material_by_id(p, *mat_id);
	if (!mat)
		return (E_MATERIAL);
	if (!mat->is_emissive)
		return (E_EMISSIVE);
	return (E_OK);
}

/**
 * Point Light: L <position> <intensity> <color> [radius] [mat_id]
 * radius defaults to 0.5 if not specified (area light with soft shadows).
 * mat_id defaults to 0 if not specified.
 */
t_error	parse_light(t_context *ctx, t_parser *p, char **tokens)
{
	t_light		light;
	int			token_count;
	uint32_t	mat_id;
	t_error		err;

	token_count = count_tokens(tokens);
	if (token_count < 4 || token_count > 6)
		return (E_ARGS);
	light = (t_light){0};
	err = parse_light_core(tokens, &light, token_count);
	if (err != E_OK)
		return (err);
	if (token_count == 6)
	{
		err = parse_light_mat(p, tokens, &mat_id);
		if (err != E_OK)
			return (err);
	}
	else
		mat_id = default_emissive_mat(ctx, &light);
	if (light.radius <= 0.0f)
		light.radius = 0.01f;
	init_point_light(ctx, &light, mat_id);
	p->has_light = true;
	return (E_OK);
}
