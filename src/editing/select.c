/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:47:57 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/27 20:26:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editing.h"
#include "scene.h"
#include "utils.h"
#include "objects.h"

static inline void	select_obj(\
t_context *ctx, t_renderer *r, t_ray *ray, t_hit *hit);

void	select_object(t_context *ctx)
{
	t_ray			ray;
	t_renderer		*r;
	t_vec2i			mouse;
	t_vec3			pixel_loc;
	t_hit			hit;

	r = &ctx->renderer;
	mlx_get_mouse_pos(ctx->mlx, &mouse.x, &mouse.y);
	pixel_loc = vec3_add(r->cam.viewport.pixel_00_loc, \
vec3_add(vec3_scale(r->cam.viewport.d_u, (float)mouse.x + 0.5f), \
vec3_scale(r->cam.viewport.d_v, (float)mouse.y + 0.5f)));
	ray = new_ray(r->cam.transform.pos, \
vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos)));
	hit = new_hit(0);
	if ((int)hit_bvh_editing(ctx->scene.geo.bvh_root_idx, &ray, &hit, \
ctx->scene.geo.bvh_nodes) | (int)hit_planes(ctx, &ray, &hit))
		select_obj(ctx, r, &ray, &hit);
	else if (ctx->editor.selected_obj && \
!(int)hit_object(ctx->editor.selected_obj, &ray, &hit))
		deselect_object(ctx, r);
}

static inline void	select_obj(\
t_context *ctx, t_renderer *r, t_ray *ray, t_hit *hit)
{
	t_camera	*cam;
	bool		rebuild_bvh;

	cam = &ctx->scene.cam;
	if (hit_object(ctx->editor.selected_obj, ray, hit) || \
hit->obj == ctx->scene.env.dir_light.obj)
		return ;
	rebuild_bvh = false;
	if (ctx->editor.selected_obj)
	{
		if (ctx->editor.selected_obj->type == OBJ_PLANE)
			vector_try_add(ctx, &ctx->scene.geo.planes, ctx->editor.selected_obj);
		else
		{
			vector_try_add(ctx, &ctx->scene.geo.objs, ctx->editor.selected_obj);
			rebuild_bvh = true;
		}
	}
	if (hit->obj->type == OBJ_PLANE)
		vector_remove(&ctx->scene.geo.planes, hit->obj);
	else
	{
		vector_remove(&ctx->scene.geo.objs, hit->obj);
		rebuild_bvh = true;
	}
	ctx->editor.selected_obj = hit->obj;
	cam->distance = \
fmaxf(vec3_dist(cam->transform.pos, hit->obj->transform.pos), 0.01f);
	if (rebuild_bvh && !init_bvh(ctx))
	{
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	}
}

bool	deselect_object(t_context *ctx, t_renderer *r)
{
	t_object	*obj;

	obj = ctx->editor.selected_obj;
	if (!obj)
		return (false);
	ctx->editor.selected_obj = NULL;
	if (obj->type == OBJ_PLANE)
	{
		vector_try_add(ctx, &ctx->scene.geo.planes, obj);
		return (true);
	}
	vector_try_add(ctx, &ctx->scene.geo.objs, obj);
	if (!init_bvh(ctx))
	{
		pthread_mutex_unlock(&r->mutex);
		fatal_error(ctx, errors(ERR_BVH), __FILE__, __LINE__);
	}
	return (true);
}
