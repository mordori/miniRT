/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing_preview.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:59 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:50:00 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

static inline t_vec3	tonemap_aces_preview(t_vec3 color);

t_vec3	post_process_preview(const t_context *ctx, t_vec3 c)
{
	c = vec3_max(c, 0.0f);
	c = vec3_scale(c, ctx->scene.cam.exposure * 0.565f);
	c = tonemap_aces_preview(c);
	c = vec3_sqrt(c);
	return (c);
}

static inline t_vec3	tonemap_aces_preview(t_vec3 c)
{
	const t_v4sf	a = c.v * (2.51f * c.v + 0.03f);
	const t_v4sf	b = c.v * (2.43f * c.v + 0.59f) + 0.14f;
	t_vec3			res;

	res.v = a / b;
	return (res);
}
