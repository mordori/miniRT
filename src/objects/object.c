/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:53:28 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/27 20:25:08 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "materials.h"
#include "objects.h"
#include "parsing.h"
#include "utils.h"

t_error	add_object(t_context *ctx, t_object *obj)
{
	t_object	*new_obj;

	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		return (E_MALLOC);
	obj->transform.rot.w = 1.0f;
	obj->transform.scale = vec3_n(1.0f);
	update_transform(&obj->transform);
	*new_obj = *obj;
	new_obj->mat = \
((t_material **)ctx->scene.assets.materials.items)[obj->material_id];
	new_obj->flags = new_obj->mat->flags;
	if (!vector_add(&ctx->scene.geo.objs, new_obj))
	{
		free(new_obj);
		return (E_MALLOC);
	}
	return (E_OK);
}

bool	hit_object(const t_object *obj, const t_ray *ray, t_hit *hit)
{
	bool		result;
	t_ray		r;

	if (!obj)
		return (false);
	r = ray_world_to_object(ray, &obj->transform.world_to_object);
	if (obj->type == OBJ_SPHERE)
		result = hit_sphere(&obj->shape, &r, hit);
	else if (obj->type == OBJ_CYLINDER)
		result = hit_cylinder(&obj->shape, &r, hit);
	else if (obj->type == OBJ_CONE)
		result = hit_cone(&obj->shape, &r, hit);
	else if (obj->type == OBJ_QUAD)
		result = hit_quad(&obj->shape, &r, hit);
	else
		return (false);
	if (result)
	{
		hit->obj = (t_object *)obj;
		hit_object_to_world(hit, &obj->transform);
	}
	return (result);
}
