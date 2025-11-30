/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_colors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:44:18 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/31 02:39:33 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_colors.h"
#include "libft_math.h"

/**
 * Interpolates two colors by first separating the color channels,
 * then interpolates them, and finally combines everything back into a 32-bit
 * RGBA color.
 *
 * @param c1 Starting color (32-bit RBGA).
 * @param c2 Target color (32-bit RBGA).
 * @param t Interpolation factor.
 * @return Interpolated color (32-bit RGBA).
 */
uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t)
{
	t_color	color1;
	t_color	color2;
	t_color	color;

	color1.r = (c1 >> 24) & 0xFF;
	color1.g = (c1 >> 16) & 0xFF;
	color1.b = (c1 >> 8) & 0xFF;
	color1.a = c1 & 0xFF;
	color2.r = (c2 >> 24) & 0xFF;
	color2.g = (c2 >> 16) & 0xFF;
	color2.b = (c2 >> 8) & 0xFF;
	color2.a = c2 & 0xFF;
	color.r = (uint8_t)ft_lerp(color1.r, color2.r, t);
	color.g = (uint8_t)ft_lerp(color1.g, color2.g, t);
	color.b = (uint8_t)ft_lerp(color1.b, color2.b, t);
	color.a = (uint8_t)ft_lerp(color1.a, color2.a, t);
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
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

	abgr.a = (rgba & 0xFF) << 24;
	abgr.b = (rgba & 0xFF00) << 8;
	abgr.g = (rgba & 0xFF0000) >> 8;
	abgr.r = (rgba & 0xFF000000) >> 24;
	return (abgr.a | abgr.b | abgr.g | abgr.r);
}
