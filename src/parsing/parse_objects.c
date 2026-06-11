/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "objects.h"
#include "parsing.h"

/**
 * Sphere: sp <center> <diameter> <color|mat_id>
 */
t_error parse_sphere(t_context* ctx, t_parser* p, char** tokens) {
	t_vec3 center;
	float diameter;
	uint32_t mat_id;
	t_error err;

	if (count_tokens(tokens) != 4)
		return (E_ARGS);
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

// qu <q> <u> <v> <color|mat_id>
t_error parse_quad(t_context* ctx, t_parser* p, char** tokens) {
	uint32_t tc;
	uint32_t mat_id;
	t_quad quad;
	t_error err;

	tc = count_tokens(tokens);
	if (tc != 5)
		return (E_ARGS);
	memset(&quad, 0, sizeof(t_quad));
	if (!parse_vec3(tokens[1], &quad.q))
		return (E_INVALID_NUM);
	if (!parse_vec3(tokens[2], &quad.u))
		return (E_INVALID_NUM);
	if (!parse_vec3(tokens[3], &quad.v))
		return (E_INVALID_NUM);
	err = resolve_material(ctx, p, tokens[4], &mat_id);
	if (err != E_OK)
		return (err);
	p->has_quad = true;
	return (init_quad(ctx, &quad, mat_id));
}
