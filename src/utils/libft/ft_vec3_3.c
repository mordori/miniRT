/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 00:03:20 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/13 00:41:24 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

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
