/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:26 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/27 20:23:20 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "parsing.h"
#include "materials.h"
#include "libft_str.h"

bool	validate_normalized(t_vec3 *vec)
{
	float	length;

	length = vec3_length(*vec);
	if (length < 0.0001f)
		return (false);
	*vec = vec3_normalize(*vec);
	return (true);
}

bool	validate_range(float value, float min, float max)
{
	return (value >= min && value <= max);
}

bool	is_color_token(const char *str)
{
	return (str && ft_strchr(str, ',') != NULL);
}

void	validate_scene(t_context *ctx, t_parser *p)
{
	if (!p->has_camera)
		fatal_error(ctx, "Missing camera (C)", ctx->file, p->line_num);
}
