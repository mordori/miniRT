/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:20:31 by myli-pen          #+#    #+#             */
/*   Updated: 2026/04/08 15:50:05 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "rendering.h"
#include "camera.h"
#include "editing.h"
#include "scene.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_context		*ctx;
	bool			dirty;

	ctx = (t_context *)param;
	dirty = false;
	pthread_mutex_lock(&ctx->renderer.mutex);
	check_edit_keys(ctx, keydata, &dirty);
	if (keydata.key == MLX_KEY_H && keydata.action == MLX_PRESS)
		ctx->hide_stats = !ctx->hide_stats;
	if (ctx->renderer.mode != SOLID && keydata.key == MLX_KEY_R && \
keydata.action == MLX_PRESS)
		dirty = reset_camera(ctx);
	pthread_mutex_unlock(&ctx->renderer.mutex);
	if (ctx->renderer.mode != SOLID && keydata.key == MLX_KEY_T && \
keydata.action == MLX_PRESS)
		set_default_view(ctx);
	if (config_renderer(ctx, keydata))
		dirty = true;
	if (ctx->renderer.mode != SOLID && keydata.key == MLX_KEY_Y && \
keydata.action == MLX_PRESS)
		screenshot(ctx);
	if (dirty)
		atomic_store(&ctx->renderer.render_cancel, true);
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
}

void	mouse_hook(\
mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_context		*ctx;
	t_renderer		*r;

	mods = 0;
	ctx = (t_context *)param;
	r = &ctx->renderer;
	pthread_mutex_lock(&r->mutex);
	if (r->mode == SOLID)
	{
		while (r->threads_running)
			pthread_cond_wait(&r->cond, &r->mutex);
		if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && \
r->cam.state == CAM_DEFAULT && !mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT))
		{
			if (ctx->editor.mode == EDIT_DEFAULT)
				select_object(ctx);
			else
				apply_edit_action(ctx);
			mods = 1;
		}
		else if (button == MLX_MOUSE_BUTTON_RIGHT && action == MLX_PRESS && \
ctx->editor.mode != EDIT_DEFAULT && \
!mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT))
			mods = cancel_edit_action(ctx);
	}
	if (mods)
		atomic_store(&ctx->renderer.render_cancel, true);
	pthread_mutex_unlock(&r->mutex);
}

void	resize_hook(int width, int height, void *param)
{
	t_context	*ctx;
	t_renderer	*r;
	bool		is_pending;

	ctx = (t_context *)param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	r = &ctx->renderer;
	is_pending = false;
	pthread_mutex_lock(&r->mutex);
	if (r->active)
	{
		is_pending = r->resize_pending;
		r->resize_pending = true;
		r->new_width = width;
		r->new_height = height;
		ctx->resize_time = time_now();
		if (!is_pending)
			pthread_cond_broadcast(&r->cond);
	}
	pthread_mutex_unlock(&r->mutex);
}
