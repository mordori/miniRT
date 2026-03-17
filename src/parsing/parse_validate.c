/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:26 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:27 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "materials.h"

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
