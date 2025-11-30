/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 03:33:53 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 02:29:04 by myli-pen         ###   ########.fr       */
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
	t_vec3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
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
	t_vec3	result;

	result.x = n;
	result.y = n;
	result.z = n;
	return (result);
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
	t_vec3	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
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
	t_vec3	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
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
t_vec3	vec3_scale(t_vec3 v, float s)
{
	t_vec3	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}
