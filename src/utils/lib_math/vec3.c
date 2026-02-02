/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 03:33:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 12:48:43 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3(const float x, const float y, const float z)
{
	return ((t_vec3){{x, y, z, 0.0f}});
}

t_vec3	vec3_n(const float n)
{
	return ((t_vec3){{n, n, n, 0.0f}});
}

t_vec3	vec3_add(const t_vec3 a, const t_vec3 b)
{
	t_vec3		res;

	res.v = a.v + b.v;
	return (res);
}

t_vec3	vec3_sub(const t_vec3 a, const t_vec3 b)
{
	t_vec3		res;

	res.v = a.v - b.v;
	return (res);
}

t_vec3	vec3_scale(const t_vec3 vec, const float s)
{
	t_vec3		factor;

	factor.v = v4sf_n(s);
	return (vec3_mul(vec, factor));
}
