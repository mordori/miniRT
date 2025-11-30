/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 01:30:53 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 02:17:49 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Constructs a 2D vector from individual x and y components.
 *
 * @param x X-component of the vector.
 * @param y Y-component of the vector.
 * @return 2D vector with the specified components.
 */
t_vec2	vec2(float x, float y)
{
	t_vec2	result;

	result.x = x;
	result.y = y;
	return (result);
}

/**
 * Constructs a 2D vector with both components set to the same value.
 *
 * Useful for initializing uniform vectors.
 *
 * @param n The value to assign to both x and y components.
 * @return 2D vector with x = y = n.
 */
t_vec2	vec2_n(float n)
{
	t_vec2	result;

	result.x = n;
	result.y = n;
	return (result);
}

/**
 * Adds two 2D vectors component-wise.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return Resulting vector after adding `a` and `b`.
 */
t_vec2	vec2_add(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

/**
 * Subtracts one 2D vector from another component-wise.
 *
 * @param a Minuend vector.
 * @param b Subtrahend vector.
 * @return Resulting vector after subtracting `b` from `a`.
 */
t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

/**
 * Scales a 2D vector by a scalar.
 *
 * Multiplies both x and y components by the given scalar value.
 *
 * @param v Input vector to scale.
 * @param s Scalar value to multiply with.
 * @return Scaled vector.
 */
t_vec2	vec2_scale(t_vec2 v, float s)
{
	t_vec2	result;

	result.x = v.x * s;
	result.y = v.y * s;
	return (result);
}
