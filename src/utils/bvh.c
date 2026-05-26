/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:20:58 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:44:07 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	sort_bvh_objects(t_bvh_node *node, const t_object **objs, size_t n)
{
	t_vec3		size;

	size = vec3_sub(node->aabb.max, node->aabb.min);
	node->axis = 0;
	if (size.y > size.x && size.y > size.z)
		node->axis = 1;
	else if (size.z > size.x && size.z > size.y)
		node->axis = 2;
	if (node->axis == 0)
		qsort(objs, n, sizeof(t_object *), cmp_bounds_x);
	else if (node->axis == 1)
		qsort(objs, n, sizeof(t_object *), cmp_bounds_y);
	else
		qsort(objs, n, sizeof(t_object *), cmp_bounds_z);
}

void	branch_idx(const t_ray *ray, const t_bvh_node *node, uint32_t *stack, int32_t *i)
{
	uint32_t		mask;

	mask = 0u - (uint32_t)(ray->signs[node->axis]);
	stack[(*i)++] = (node->left_idx & mask) | (node->right_idx & ~mask);
	stack[(*i)++] = (node->right_idx & mask) | (node->left_idx & ~mask);
}
