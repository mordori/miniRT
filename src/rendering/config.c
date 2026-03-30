/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:49 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:49:51 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "utils.h"
#include "scene.h"
#include "editing.h"

static inline bool	set_render_mode(\
t_context *ctx, t_renderer *r, mlx_key_data_t keydata);
static inline void	set_samples(t_context *ctx, mlx_key_data_t keydata);
static inline bool	set_bounces(t_context *ctx, mlx_key_data_t keydata);
static inline void	reset_editor(t_context *ctx, t_renderer *r);

bool	config_renderer(t_context *ctx, mlx_key_data_t keydata)
{
	bool		dirty;

	dirty = false;
	if (set_render_mode(ctx, &ctx->renderer, keydata))
		dirty = true;
	if (set_bounces(ctx, keydata))
		dirty = true;
	set_samples(ctx, keydata);
	return (dirty);
}

static inline bool	set_render_mode(\
t_context *ctx, t_renderer *r, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
	{
		pthread_mutex_lock(&r->mutex);
		while (r->threads_running)
			pthread_cond_wait(&r->cond, &r->mutex);
		if (r->mode != SOLID)
			r->mode = SOLID;
		else
		{
			if (ctx->editor.selected_obj)
				reset_editor(ctx, r);
			else if (ctx->editor.mode != EDIT_DEFAULT)
				end_edit_action(ctx);
			ctx->editor.mode = EDIT_DEFAULT;
			r->mode = RENDERED;
		}
		pthread_mutex_unlock(&r->mutex);
		return (true);
	}
	return (false);
}

static inline void	reset_editor(t_context *ctx, t_renderer *r)
{
	t_object	*obj;

	if (ctx->editor.mode != EDIT_DEFAULT)
		cancel_edit_action(ctx);
	obj = ctx->editor.selected_obj;
	ctx->editor.selected_obj = NULL;
	if (obj->type == OBJ_PLANE)
		vector_try_add(ctx, &ctx->scene.geo.planes, obj);
	else
	{
		vector_try_add(ctx, &ctx->scene.geo.objs, obj);
		if (!init_bvh(ctx))
		{
			pthread_mutex_unlock(&r->mutex);
			fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
		}
	}
	memset(ctx->editor.selection_mask, 0, sizeof(float) * r->pixels);
}

static inline void	set_samples(t_context *ctx, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_O && keydata.action == MLX_PRESS)
	{
		ctx->renderer.render_samples >>= 1u;
		if (ctx->renderer.render_samples < 2u)
			ctx->renderer.render_samples = 2u;
	}
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_PRESS)
	{
		ctx->renderer.render_samples <<= 1u;
		if (ctx->renderer.render_samples > 8192u)
			ctx->renderer.render_samples = 8192u;
	}
}

static inline bool	set_bounces(t_context *ctx, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_U && keydata.action == MLX_PRESS)
	{
		ctx->renderer.render_bounces >>= 1u;
		if (ctx->renderer.render_bounces < 2u)
			ctx->renderer.render_bounces = 2u;
		else
			return (true);
	}
	if (keydata.key == MLX_KEY_I && keydata.action == MLX_PRESS)
	{
		ctx->renderer.render_bounces <<= 1u;
		if (ctx->renderer.render_bounces > 128u)
			ctx->renderer.render_bounces = 128u;
		else
			return (true);
	}
	return (false);
}
