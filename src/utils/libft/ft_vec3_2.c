/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:28:02 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/11 22:13:42 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

/**
 * Computes the cross product of two 3D vectors.
 *
 * The result is a vector that is perpendicular to both input vectors.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return Vector perpendicular to both `a` and `b`.
 */
t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	}});
}

/**
 * Computes the dot product of two 3D vectors.
 *
 * Measures the similarity in direction of two vectors.
 *
 * @param a First input vector.
 * @param b Second input vector.
 * @return Scalar dot product of `a` and `b`.
 */
float	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (
		a.x * b.x +
		a.y * b.y +
		a.z * b.z
	);
}

t_vec3	vec3_div(t_vec3 v, float div)
{
	if (div < 1e-6f)
		return (vec3_n(0.0f));
	return ((t_vec3){{
		v.x / div,
		v.y / div,
		v.z / div
	}});
}

/**
 * Normalizes a 3D vector.
 *
 * Converts the vector to unit length (length = 1), preserving direction.
 * If the input vector has zero length, a zero vector is returned.
 *
 * @param v Input vector to normalize.
 * @return Normalized unit vector, or a zero vector if `v` has zero length.
 */
t_vec3	vec3_normalize(t_vec3 v)
{
	return (vec3_div(v, vec3_length(v)));
}

/**
 * Computes the length (magnitude) of a 3D vector.
 *
 * @param v The input vector.
 * @return The scalar length of vector `v`.
 */
float	vec3_length(t_vec3 v)
{
	return (sqrtf(vec3_dot(v, v)));
}
