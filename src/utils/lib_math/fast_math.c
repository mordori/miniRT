/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fast_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:48:25 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:48:27 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

/**
 * Fast atan2f approximation using minimax polynomial.
 * Max error: ~0.005 radians (~0.3 degrees). Invisible in path tracing.
 * Replaces glibc atan2f (~760M Ir savings in spherical_uv).
 */
float	fast_atan2f(float y, float x)
{
	float	abs_x;
	float	abs_y;
	float	a;
	float	s;
	float	r;

	abs_x = fabsf(x) + 1e-10f;
	abs_y = fabsf(y);
	a = fminf(abs_x, abs_y) / fmaxf(abs_x, abs_y);
	s = a * a;
	r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
	if (abs_y > abs_x)
		r = (float)M_PI_2 - r;
	if (x < 0.0f)
		r = (float)M_PI - r;
	if (y < 0.0f)
		r = -r;
	return (r);
}

/**
 * Fast acosf approximation using polynomial fitting.
 * Max error: ~0.02 radians (~1 degree). Good enough for UV mapping.
 * Replaces glibc acosf (~310M Ir savings in spherical_uv).
 */
float	fast_acosf(float x)
{
	float	negate;
	float	ret;

	negate = (float)(x < 0.0f);
	x = fabsf(x);
	ret = -0.0187293f;
	ret = ret * x + 0.0742610f;
	ret = ret * x - 0.2121144f;
	ret = ret * x + 1.5707288f;
	ret = ret * sqrtf(1.0f - x);
	ret = ret - 2.0f * negate * ret;
	return (negate * (float)M_PI + ret);
}

/**
 * Fast sRGB gamma approximation: powf(x, 1/2.2) ≈ x^0.4545
 * Uses 4-term polynomial in x^0.25, x^0.5, x^0.75, and x.
 * Computed using s * q for a "free" x^0.75 term.
 * Max error: ~0.3%.
 */
float	fast_srgb(float x)
{
	float	s;
	float	q;
	float	sq;

	if (x <= 0.0f)
		return (0.0f);
	s = sqrtf(x);
	q = sqrtf(s);
	sq = s * q;
	return (0.04400f * q + 1.17400f * s - 0.28500f * sq + 0.06700f * x);
}
