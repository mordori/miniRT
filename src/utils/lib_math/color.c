/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:44:18 by myli-pen          #+#    #+#             */
/*   Updated: 2026/02/06 04:27:41 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"

t_vec3	lerp_color(uint32_t c1, uint32_t c2, float t)
{
	t_v4sf	a;
	t_v4sf	b;
	t_vec3	res;

	a = v4sf((c1 >> 24) & 0xFF, (c1 >> 16) & 0xFF, (c1 >> 8) & 0xFF, 255.0f);
	b = v4sf((c2 >> 24) & 0xFF, (c2 >> 16) & 0xFF, (c2 >> 8) & 0xFF, 255.0f);
	res.v = ((1.0f - t) * a + t * b) * INV_255F;
	return (res);
}

/**
 * Converts a 32-bit RGBA color value to ABGR format.
 *
 * It does so by extracting each channel using bitmasks and shifts,
 * then reassembling them in the new order.
 *
 * @param rgba 32-bit unsigned integer in RGBA format.
 * @return 32-bit unsigned integer in ABGR format.
 */
uint32_t	rgba_to_abgr(uint32_t rgba)
{
	t_color	abgr;

	abgr.r = (rgba & 0xFF) << 24;
	abgr.g = (rgba & 0xFF00) << 8;
	abgr.b = (rgba & 0xFF0000) >> 8;
	abgr.a = (rgba & 0xFF000000) >> 24;
	return (abgr.full);
}
