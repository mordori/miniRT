/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:55:00 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/05 22:17:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	vec3_negate(t_vec3 vec)
{
	return (vec3_scale(vec, -1.0f));
}

t_vec3	vec3_lerp(t_vec3 a, t_vec3 b, float t)
{
	t_vec3		res;

	res.v = a.v + (b.v - a.v) * v4sf_n(t);
	return (res);
}
