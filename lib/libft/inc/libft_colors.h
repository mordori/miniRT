/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_colors.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:45:12 by myli-pen          #+#    #+#             */
/*   Updated: 2025/12/01 20:39:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_defs.h"

union u_color
{
	uint32_t	full;
	struct
	{
		uint8_t	r;
		uint8_t	g;
		uint8_t	b;
		uint8_t	a;
	};
};

uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t);
uint32_t	rgba_to_abgr(uint32_t rgba);
