/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:21:13 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/31 19:23:34 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "editing.h"
#include "camera.h"

void	check_edit_keys(t_context *ctx, mlx_key_data_t keydata, bool *dirty)
{
	if (ctx->renderer.mode == SOLID)
	{
		while (ctx->renderer.threads_running)
			pthread_cond_wait(&ctx->renderer.cond, &ctx->renderer.mutex);
		if (config_editor(ctx, keydata))
			*dirty = true;
		if (ctx->editor.mode == EDIT_DEFAULT && keydata.key == MLX_KEY_F && keydata.action == MLX_PRESS)
			*dirty = frame_camera(ctx, ctx->editor.selected_obj);
		if (keydata.key == MLX_KEY_Q && keydata.action == MLX_PRESS)
			*dirty = deselect_object(ctx, &ctx->renderer);
	}
}
