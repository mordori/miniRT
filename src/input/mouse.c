/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:48:08 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/27 20:26:11 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

t_vec2i	get_mouse_delta(t_context *ctx)
{
	t_vec2i		delta;
	int32_t		w;
	int32_t		h;

	mlx_get_mouse_pos(ctx->mlx, &ctx->mouse.pos.x, &ctx->mouse.pos.y);
	delta = vec2i_sub(ctx->mouse.pos, ctx->mouse.pos_prev);
	if (ctx->editor.mode == EDIT_DEFAULT && ctx->scene.cam.state == CAM_DEFAULT)
		return (delta);
	w = (int32_t)ctx->img->width;
	h = (int32_t)ctx->img->height;
	if (\
delta.x > w / 2 || delta.x < -w / 2 || delta.y > h / 2 || delta.y < -h / 2)
		delta = vec2i_n(0);
	if (ctx->mouse.pos.x >= w - 1)
		ctx->mouse.pos.x = 1;
	else if (ctx->mouse.pos.x <= 0)
		ctx->mouse.pos.x = w - 2;
	if (ctx->mouse.pos.y >= h - 1)
		ctx->mouse.pos.y = 1;
	else if (ctx->mouse.pos.y <= 0)
		ctx->mouse.pos.y = h - 2;
	mlx_set_mouse_pos(ctx->mlx, ctx->mouse.pos.x, ctx->mouse.pos.y);
	ctx->mouse.pos_prev = ctx->mouse.pos;
	return (delta);
}
