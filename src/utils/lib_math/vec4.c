/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:46:02 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:31:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec4	vec4(float x, float y, float z, float w)
{
	return ((t_vec4){{x, y, z, w}});
}

t_vec4	vec4_n(float n)
{
	return ((t_vec4){{n, n, n, n}});
}

t_vec4	vec4_add(t_vec4 a, t_vec4 b)
{
	t_vec4		res;

	res.v = a.v + b.v;
	return (res);
}

t_vec4	vec4_sub(t_vec4 a, t_vec4 b)
{
	t_vec4		res;

	res.v = a.v - b.v;
	return (res);
}

t_vec4	vec4_scale(t_vec4 vec,  float s)
{
	t_vec4		res;
	t_v4sf		factor;

	factor = v4sf_n(s);
	res.v = vec.v * factor;
	return (res);
}
