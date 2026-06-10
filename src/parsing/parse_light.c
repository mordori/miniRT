/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "lights.h"
#include "materials.h"
#include "parsing.h"

static t_error parse_light_core(char** tokens, t_light* light, int tc) {
	t_vec3 position;
	t_vec3 color;
	float ratio;

	if (!parse_vec3(tokens[1], &position))
		return (E_INVALID_NUM);
	if (!parse_float(tokens[2], &ratio))
		return (E_INVALID_NUM);
	if (!validate_range(ratio, 0.0f, 1.0f))
		return (E_RANGE);
	if (!parse_color(tokens[3], &color))
		return (E_INVALID_COLOR);
	if (tc >= 5 && !parse_float(tokens[4], &light->radius))
		return (E_INVALID_NUM);
	if (tc >= 5 && !validate_range(light->radius, 0.01f, 1000000.0f))
		return (E_RANGE);
	light->type = LIGHT_POINT;
	light->pos = position;
	light->intensity = ratio;
	light->color = color;
	return (E_OK);
}

static t_error default_emissive_mat(t_context* ctx, t_light* light, uint32_t* out_id) {
	t_material mat;

	mat = (t_material){ 0 };
	mat.albedo = light->color;
	mat.emission = vec3_scale(light->color, 50 * light->intensity);
	mat.is_emissive = light->intensity > 0.0f;
	mat.flags = 2;
	return (new_material(ctx, &mat, out_id));
}

static t_error parse_light_mat(t_parser* p, char** tokens, uint32_t* mat_id) {
	t_material* mat;

	if (!parse_uint(tokens[5], mat_id))
		return (E_RANGE);
	mat = get_material_by_id(p, *mat_id);
	if (!mat)
		return (E_MATERIAL);
	*mat_id += 1;
	return (E_OK);
}

/**
 * Point Light: L <position> <intensity> <color> [radius] [mat_id]
 */
t_error parse_light(t_context* ctx, t_parser* p, char** tokens) {
	t_light light;
	int tc;
	uint32_t mat_id;
	t_error err;

	tc = count_tokens(tokens);
	if (tc < 4 || tc > 6)
		return (E_ARGS);
	memset(&light, 0, sizeof(t_light));
	err = parse_light_core(tokens, &light, tc);
	if (err == E_OK && tc == 6)
		err = parse_light_mat(p, tokens, &mat_id);
	else if (err == E_OK)
		err = default_emissive_mat(ctx, &light, &mat_id);
	if (err != E_OK)
		return (err);
	if (light.radius <= 0.0f)
		light.radius = 1.0f;
	err = init_point_light(ctx, &light, mat_id);
	if (err == E_OK)
		p->has_light = true;
	return (err);
}
