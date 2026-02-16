/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 01:30:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/15 21:27:27 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec2	vec2(float x, float y)
{
	return ((t_vec2){{x, y}});
}

t_vec2	vec2_n(float n)
{
	return ((t_vec2){{n, n}});
}

t_vec2	vec2_add(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){{
		a.x + b.x,
		a.y + b.y
	}});
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){{
		a.x - b.x,
		a.y - b.y
	}});
}

t_vec2	vec2_scale(t_vec2 vec, float s)
{
	return ((t_vec2){{
		vec.x * s,
		vec.y * s
	}});
}
