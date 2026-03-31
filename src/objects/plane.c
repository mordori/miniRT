/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:53:36 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/31 19:07:50 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "materials.h"
#include "utils.h"

t_error	init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, uint32_t mat_id)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (E_MALLOC);
	*obj = (t_object){0};
	obj->type = OBJ_PLANE;
	obj->transform.pos = point;
	obj->transform.rot = quat_from_dir(normal);
	obj->transform.scale = g_one;
	update_transform(&obj->transform);
	update_bounds(obj);
	onb(g_up, &obj->shape.plane.u, &obj->shape.plane.v);
	obj->shape.plane.point = g_zero;
	obj->shape.plane.normal = g_up;
	obj->shape.plane.d = 0.0f;
	obj->material_id = mat_id;
	obj->mat = ((t_material **)ctx->scene.assets.materials.items)[mat_id];
	obj->flags = obj->mat->flags;
	if (!vector_add(&ctx->scene.geo.planes, obj))
	{
		free(obj);
		return (E_MALLOC);
	}
	return (E_OK);
}

bool	hit_planes(const t_context *ctx, const t_ray *ray, t_hit *hit)
{
	size_t		idx;
	bool		res;
	t_object	*obj;
	t_ray		r;

	res = false;
	idx = ctx->scene.geo.planes.total;
	while (idx--)
	{
		obj = ((t_object **)ctx->scene.geo.planes.items)[idx];
		r = ray_world_to_object(ray, &obj->transform.world_to_object);
		if (!(obj->flags & OBJ_HIDDEN_SCENE) && !(hit->is_primary && \
(obj->flags & OBJ_HIDDEN_CAM)) && hit_plane(&obj->shape, &r, hit))
		{
			hit->obj = obj;
			hit_object_to_world(hit, &obj->transform);
			res = true;
		}
	}
	return (res);
}

bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	float		denom;
	float		t;

	denom = ray->dir.y;
	if (fabsf(denom) < G_EPSILON)
		return (false);
	t = -ray->origin.y / denom;
	if (t < G_EPSILON || t >= hit->t)
		return (false);
	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	hit->normal = g_up;
	if (denom > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	hit->uv.u = vec3_dot(hit->point, shape->plane.u);
	hit->uv.v = vec3_dot(hit->point, shape->plane.v);
	hit->tangent = shape->plane.u;
	hit->bitangent = shape->plane.v;
	return (true);
}
