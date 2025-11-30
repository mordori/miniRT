/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2i.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:32:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 02:31:48 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Constructs a 2D integer vector from integer components.
 *
 * @param x X-component of the vector.
 * @param y Y-component of the vector.
 * @return 2D integer vector with the specified components.
 */
t_vec2i	vec2i(int x, int y)
{
	t_vec2i	result;

	result.x = x;
	result.y = y;
	return (result);
}

/**
 * Constructs a 2D integer vector with both components set to
 * the same value.
 *
 * @param n Value to be assigned to both x and y.
 * @return 2D integer vector with x = y = n.
 */
t_vec2i	vec2i_n(int n)
{
	t_vec2i	result;

	result.x = n;
	result.y = n;
	return (result);
}

/**
 * Adds two 2D integer vectors component-wise.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return Resulting vector after adding `a` and `b`.
 */
t_vec2i	vec2i_add(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

/**
 * Subtracts one 2D integer vector from another component-wise.
 *
 * @param a Minuend vector.
 * @param b Subtrahend vector.
 * @return Resulting vector after subtracting `b` from `a`.
 */
t_vec2i	vec2i_sub(t_vec2i a, t_vec2i b)
{
	t_vec2i	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

/**
 * Scales a 2D integer vector by a float scalar.
 *
 * The vector components are cast to float, multiplied by `s`,
 * then truncated back to integers.
 *
 * @param v Input vector to scale.
 * @param s Scalar multiplier.
 * @return Scaled vector with truncated integer components.
 */
t_vec2i	vec2i_scale(t_vec2i v, float s)
{
	t_vec2i	result;

	result.x = (int)((float)v.x * s);
	result.y = (int)((float)v.y * s);
	return (result);
}
