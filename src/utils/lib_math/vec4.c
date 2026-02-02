/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:46:02 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/01 12:48:00 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec4	vec4(const float x, const float y, const float z, const float w)
{
	return ((t_vec4){{x, y, z, w}});
}

t_vec4	vec4_n(const float n)
{
	return ((t_vec4){{n, n, n, n}});
}

t_vec4	vec4_add(const t_vec4 a, const t_vec4 b)
{
	t_vec4		res;

	res.v = a.v + b.v;
	return (res);
}

t_vec4	vec4_sub(const t_vec4 a, const t_vec4 b)
{
	t_vec4		res;

	res.v = a.v - b.v;
	return (res);
}

t_vec4	vec4_scale(const t_vec4 vec, const float s)
{
	t_vec4		res;
	t_v4sf		factor;

	factor = v4sf_n(s);
	res.v = vec.v * factor;
	return (res);
}
