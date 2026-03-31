/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 19:21:01 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/31 20:01:39 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "scene.h"

int	cmp_bounds_x(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;
	int			mask_1;
	int			mask_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	mask_1 = (obj_1->bounds_center.x < obj_2->bounds_center.x);
	mask_2 = (obj_1->bounds_center.x > obj_2->bounds_center.x);
	return (-mask_1 + mask_2);
}

int	cmp_bounds_y(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;
	int			mask_1;
	int			mask_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	mask_1 = (obj_1->bounds_center.y < obj_2->bounds_center.y);
	mask_2 = (obj_1->bounds_center.y > obj_2->bounds_center.y);
	return (-mask_1 + mask_2);
}

int	cmp_bounds_z(const void *a, const void *b)
{
	t_object	*obj_1;
	t_object	*obj_2;
	int			mask_1;
	int			mask_2;

	obj_1 = *(t_object **)a;
	obj_2 = *(t_object **)b;
	mask_1 = (obj_1->bounds_center.z < obj_2->bounds_center.z);
	mask_2 = (obj_1->bounds_center.z > obj_2->bounds_center.z);
	return (-mask_1 + mask_2);
}

void	update_bounds(t_object *obj)
{
	t_aabb		aabb;

	if (obj->type == OBJ_PLANE)
		obj->bounds_center = obj->transform.pos;
	else
	{
		aabb = get_object_bounds(obj);
		obj->bounds_center = vec3_div(vec3_add(aabb.min, aabb.max), 2.0f);
		obj->bounds = vec3_sub(aabb.max, aabb.min);
	}
}

float	get_max_bounds_dim(const t_object *obj)
{
	return (fmaxf(fmaxf(obj->bounds.x, obj->bounds.y), obj->bounds.z));
}
