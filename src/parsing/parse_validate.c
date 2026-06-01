/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"
#include "parsing.h"

bool validate_normalized(t_vec3* vec) {
	float length;

	length = vec3_length(*vec);
	if (length < 0.0001f)
		return (false);
	*vec = vec3_normalize(*vec);
	return (true);
}

bool validate_range(float value, float min, float max) {
	return (value >= min && value <= max);
}

bool is_color_token(const char* str) {
	return (str && ft_strchr(str, ',') != NULL);
}

void validate_scene(t_context* ctx, t_parser* p, char** lines) {
	if (!p->has_camera)
		p_error(ctx, E_NO_CAMERA, p->line_num, lines);
}
