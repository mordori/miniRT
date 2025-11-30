/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2i_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:05:21 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 02:31:54 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Converts floating-point coordinates to a 2D integer vector.
 *
 * Casts the input float values to integers and returns them
 * as a `vec2i` vector. Truncates toward zero.
 *
 * @param x The x-component (float) to convert.
 * @param y The y-component (float) to convert.
 * @return 2D integer vector with components x and y.
 */
t_vec2i	vec2i_f(float x, float y)
{
	t_vec2i	result;

	result.x = (int)x;
	result.y = (int)y;
	return (result);
}

/**
 * Constructs a 2D integer vector with both components set to
 * the same float value.
 *
 * The float value is truncated to an integer.
 *
 * @param n Float value to be converted and assigned to both x and y.
 * @return 2D integer vector with x = y = (int)n.
 */
t_vec2i	vec2i_nf(float n)
{
	t_vec2i	result;

	result.x = (int)n;
	result.y = (int)n;
	return (result);
}
