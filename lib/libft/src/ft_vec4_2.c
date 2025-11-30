/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec4_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 23:32:25 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 02:34:24 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Constructs a 4D vector from a 3D vector and a w-component.
 *
 * Copies the x, y, and z components from the given 3D vector `v`
 * and assigns the provided `w` value as the fourth component.
 *
 * @param v Input 3D vector to supply x, y, z components.
 * @param w W-component to assign to the 4D vector.
 * @return 4D vector composed of (v.x, v.y, v.z, w).
 */
t_vec4	vec4_3(t_vec3 v, float w)
{
	t_vec4	result;

	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	result.w = w;
	return (result);
}
