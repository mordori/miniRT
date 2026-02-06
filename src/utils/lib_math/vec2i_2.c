/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2i_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:05:21 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:30:04 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

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
	return ((t_vec2i){(int32_t)x, (int32_t)y});
}

/**
 * Constructs a 2D integer vector with both components set to
 * the same float value.
 *
 * The float value is truncated to an integer.
 *
 * @param n Float value to be converted and assigned to both x and y.
 * @return 2D integer vector with x = y = (int32_t)n.
 */
t_vec2i	vec2i_nf(float n)
{
	return ((t_vec2i){(int32_t)n, (int32_t)n});
}
