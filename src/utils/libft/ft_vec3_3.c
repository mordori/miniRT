/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:03:20 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/13 21:25:27 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"
#include "libft_random.h"

t_vec3	vec3_mul(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	}});
}

t_vec3	vec3_reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_sub(v, vec3_scale(n, 2 * vec3_dot(v, n))));
}

t_vec3	vec3_unit_random(uint32_t *seed)
{
	t_vec3	result;
	float	len_sq;

	while (true)
	{
		result = ((t_vec3){{
			randomfn11(seed),
			randomfn11(seed),
			randomfn11(seed)
		}});
		len_sq = vec3_dot(result, result);
		if (len_sq <= 1.0f && fabsf(len_sq) >= M_EPSILON)
			return (vec3_div(result, sqrtf(len_sq)));
	}
}
