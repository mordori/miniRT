/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 03:33:53 by myli-pen          #+#    #+#             */
/*   Updated: 2026/01/22 20:29:37 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Constructs a 3D vector from individual x, y, and z components.
 *
 * @param x X-component of the vector.
 * @param y Y-component of the vector.
 * @param z Z-component of the vector.
 * @return 3D vector with the specified components.
 */
t_vec3	vec3(float x, float y, float z)
{
	return ((t_vec3){{x, y, z}});
}

/**
 * Constructs a 3D vector with all components set to the same value.
 *
 * Useful for initializing uniform vectors.
 *
 * @param n The value to assign to x, y, and z components.
 * @return 3D vector with x = y = z = n.
 */
t_vec3	vec3_n(float n)
{
	return ((t_vec3){{n, n, n}});
}

/**
 * Adds two 3D vectors component-wise.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return Resulting vector after adding `a` and `b`.
 */
t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	}});
}

/**
 * Subtracts one 3D vector from another component-wise.
 *
 * @param a Minuend vector.
 * @param b Subtrahend vector.
 * @return Resulting vector after subtracting `b` from `a`.
 */
t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	}});
}

/**
 * Scales a 3D vector by a scalar.
 *
 * Multiplies each component of the vector by the scalar `s`.
 *
 * @param v Input vector to scale.
 * @param s Scalar multiplier.
 * @return Scaled vector.
 */
t_vec3	vec3_scale(t_vec3 vec, float s)
{
	return ((t_vec3){{
		vec.x * s,
		vec.y * s,
		vec.z * s
	}});
}
