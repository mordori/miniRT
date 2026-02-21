/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 07:35:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/21 03:38:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

float	vec3_dist_sq(t_vec3 a, t_vec3 b)
{
	t_vec3		diff;

	diff = vec3_sub(b, a);
	return (vec3_dot(diff, diff));
}

bool	vec3_is_nan_inf(t_vec3 vec)
{
	static const uint32_t	e_bits = 0x7F800000u;
	t_v4ui_ui				v_s;

	v_s.v = (t_v4ui)vec.v;
	v_s.v &= e_bits;
	return (v_s.s[0] == e_bits | v_s.s[1] == e_bits | v_s.s[2] == e_bits);
}
