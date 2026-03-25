/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:20:14 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 19:20:15 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	random_uv(\
const t_context *ctx, t_path *path, t_pixel *pixel, t_bn_channel c)
{
	t_vec2		offset;
	uint32_t	dim_u;
	uint32_t	dim_v;

	dim_u = c + (path->bounce * ctx->bn_stride);
	dim_v = c + 1 + (path->bounce * ctx->bn_stride);
	if (path->bounce == 0)
	{
		path->uv = vec2(blue_noise(&ctx->tex_bn, pixel, dim_u), \
blue_noise(&ctx->tex_bn, pixel, dim_v));
	}
	else
	{
		offset = vec2(static_blue_noise(&ctx->tex_bn, pixel, dim_u), \
static_blue_noise(&ctx->tex_bn, pixel, dim_v));
		path->uv = \
r2_sequence(pixel->frame + (path->bounce * FP_PRIME), offset);
	}
}
