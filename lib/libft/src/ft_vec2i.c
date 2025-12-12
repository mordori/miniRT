/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2i.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:32:13 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/11 22:04:54 by myli-pen         ###   ########.fr       */
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
t_vec2i	vec2i(int32_t x, int32_t y)
{
	return ((t_vec2i){x, y});
}

/**
 * Constructs a 2D integer vector with both components set to
 * the same value.
 *
 * @param n Value to be assigned to both x and y.
 * @return 2D integer vector with x = y = n.
 */
t_vec2i	vec2i_n(int32_t n)
{
	return ((t_vec2i){n, n});
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
	return ((t_vec2i){
		a.x + b.x,
		a.y + b.y
	});
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
	return ((t_vec2i){
		a.x - b.x,
		a.y - b.y
	});
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
	return ((t_vec2i){
		(int)((float)v.x * s),
		(int)((float)v.y * s)
	});
}
