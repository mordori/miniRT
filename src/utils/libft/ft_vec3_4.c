/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 18:35:30 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/25 23:43:40 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

t_vec3	vec3_sqrt(t_vec3 vec)
{
	return ((t_vec3){{
		sqrtf(vec.r),
		sqrtf(vec.g),
		sqrtf(vec.b)
	}});
}

t_vec3	vec3_clamp01(t_vec3 vec)
{
	return ((t_vec3){{
		ft_clamp01(vec.r),
		ft_clamp01(vec.g),
		ft_clamp01(vec.b)
	}});
}

t_vec3	vec3_clamp_mag(t_vec3 vec, float max)
{
	float	len_sq;
	float	scale;

	len_sq = vec3_dot(vec, vec);
	if (len_sq > max * max)
	{
		scale = max / sqrtf(len_sq);
		return (vec3_scale(vec, scale));
	}
	return (vec);
}

t_vec3	vec3_addf(t_vec3 vec, float n)
{
	return (vec3(vec.x + n, vec.y + n, vec.z + n));
}
