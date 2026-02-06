/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 03:33:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/05 21:22:44 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3(float x, float y, float z)
{
	return ((t_vec3){{x, y, z, 0.0f}});
}

t_vec3	vec3_n(float n)
{
	return ((t_vec3){{n, n, n, 0.0f}});
}

t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3		res;

	res.v = a.v + b.v;
	return (res);
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3		res;

	res.v = a.v - b.v;
	return (res);
}

t_vec3	vec3_scale(t_vec3 vec, float s)
{
	t_vec3		factor;

	factor.v = v4sf_n(s);
	return (vec3_mul(vec, factor));
}
