/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:10:15 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:43:54 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "objects.h"
#include "utils.h"

static inline uint32_t	build_bvh(\
t_context *ctx, const t_object **objs, size_t n, uint32_t *nodes);

bool	init_bvh(t_context *ctx)
{
	t_scene		*scene;
	t_object	**objs;
	size_t		n;
	uint32_t	nodes;

	scene = &ctx->scene;
	n = scene->geo.objs.total;
	objs = (t_object **)scene->geo.objs.items;
	free(scene->geo.bvh_nodes);
	scene->geo.bvh_nodes = NULL;
	if (n == 0)
	{
		scene->geo.bvh_root_idx = 0;
		return (true);
	}
	scene->geo.bvh_nodes = malloc(sizeof(t_bvh_node) * ((2 * n) - 1));
	if (!scene->geo.bvh_nodes)
		return (false);
	nodes = 1;
	scene->geo.bvh_root_idx = \
build_bvh(ctx, (const t_object **)objs, n, &nodes);
	return (true);
}

static inline uint32_t	build_bvh(\
t_context *ctx, const t_object **objs, size_t n, uint32_t *nodes)
{
	t_bvh_node	*node;
	int32_t		idx;
	size_t		half_n;

	idx = (*nodes)++;
	node = &ctx->scene.geo.bvh_nodes[idx - 1];
	memset(node, 0, sizeof(t_bvh_node));
	if (n == 1)
	{
		node->obj = (t_object *)objs[0];
		node->aabb = get_object_bounds(node->obj);
		return (idx);
	}
	node->aabb = get_volume_bounds((t_object **)objs, n);
	sort_bvh_objects(node, objs, n);
	half_n = n / 2;
	node->left_idx = build_bvh(ctx, objs, half_n, nodes);
	node->right_idx = build_bvh(ctx, objs + half_n, n - half_n, nodes);
	return (idx);
}

bool	hit_bvh(\
uint32_t root_idx, const t_ray *ray, t_hit *hit, t_bvh_node *tree)
{
	t_bvh_element	bvh;

	if (!root_idx)
		return (false);
	bvh.res = false;
	bvh.i = 0;
	bvh.stack[bvh.i++] = root_idx;
	while (bvh.i > 0)
	{
		bvh.node = &tree[bvh.stack[--bvh.i] - 1];
		if (!hit_aabb(&bvh.node->aabb, ray, hit->t, 0))
			continue ;
		if (bvh.node->obj)
		{
			bvh.temp = *hit;
			if (!(bvh.node->obj->flags & OBJ_HIDDEN_SCENE) && \
!(hit->is_primary && (bvh.node->obj->flags & OBJ_HIDDEN_CAM)) && \
hit_object(bvh.node->obj, ray, &bvh.temp))
			{
				bvh.res = true;
				*hit = bvh.temp;
			}
			continue ;
		}
		branch_idx(ray, bvh.node, bvh.stack, &bvh.i);
	}
	return (bvh.res);
}

bool	hit_bvh_shadow(\
uint32_t root_idx, const t_ray *ray, float dist, t_bvh_node *tree)
{
	uint32_t	stack[64];
	int32_t		i;
	t_bvh_node	*node;
	t_hit		temp;

	if (!root_idx)
		return (false);
	i = 0;
	stack[i++] = root_idx;
	while (i > 0)
	{
		node = &tree[stack[--i] - 1];
		if (!hit_aabb(&node->aabb, ray, dist, 0))
			continue ;
		if (node->obj)
		{
			temp.t = dist;
			if (!(node->obj->flags & OBJ_NO_CAST_SHADOW)
				&& hit_object(node->obj, ray, &temp))
				return (true);
			continue ;
		}
		branch_idx(ray, node, stack, &i);
	}
	return (false);
}

bool	hit_bvh_editing(\
uint32_t root_idx, const t_ray *ray, t_hit *hit, t_bvh_node *tree)
{
	t_bvh_element	bvh;

	if (!root_idx)
		return (false);
	bvh.res = false;
	bvh.i = 0;
	bvh.stack[bvh.i++] = root_idx;
	while (bvh.i > 0)
	{
		bvh.node = &tree[bvh.stack[--bvh.i] - 1];
		if (!hit_aabb(&bvh.node->aabb, ray, hit->t, 0))
			continue ;
		if (bvh.node->obj)
		{
			bvh.temp = *hit;
			if (!(bvh.node->obj->flags & OBJ_HIDDEN_SCENE) && \
hit_object(bvh.node->obj, ray, &bvh.temp))
			{
				bvh.res = true;
				*hit = bvh.temp;
			}
			continue ;
		}
		branch_idx(ray, bvh.node, bvh.stack, &bvh.i);
	}
	return (bvh.res);
}
