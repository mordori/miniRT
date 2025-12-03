/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:46:02 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 21:10:37 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Constructs a 4D vector from individual x, y, z, and w components.
 *
 * @param x X-component of the vector.
 * @param y Y-component of the vector.
 * @param z Z-component of the vector.
 * @param w W-component of the vector.
 * @return 4D vector with the specified components.
 */
t_vec4	vec4(float x, float y, float z, float w)
{
	return ((t_vec4){{x, y, z, w}});
}

/**
 * Constructs a 4D vector with all components set to the same value.
 *
 * Useful for initializing uniform vectors.
 *
 * @param n The value to assign to x, y, z, and w components.
 * @return 4D vector with x = y = z = w = n.
 */
t_vec4	vec4_n(float n)
{
	return ((t_vec4){{n, n, n, n}});
}

/**
 * Adds two 4D vectors component-wise.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return Resulting vector after adding `a` and `b`.
 */
t_vec4	vec4_add(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}});
}

/**
 * Subtracts one 4D vector from another component-wise.
 *
 * @param a Minuend vector.
 * @param b Subtrahend vector.
 * @return Resulting vector after subtracting `b` from `a`.
 */
t_vec4	vec4_sub(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}});
}

/**
 * Scales a 4D vector by a scalar.
 *
 * Multiplies each component of the vector by the scalar `s`.
 *
 * @param v Input vector to scale.
 * @param s Scalar multiplier.
 * @return Scaled vector.
 */
t_vec4	vec4_scale(t_vec4 v, float s)
{
	return ((t_vec4){{v.x * s, v.y * s, v.z * s, v.w * s}});
}
