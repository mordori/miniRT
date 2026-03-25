/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:48:11 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:52:43 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lights.h"
#include "utils.h"
#include "objects.h"
#include "materials.h"

static inline t_error	init_dir_light(\
t_context *ctx, t_light *light, t_object *obj);
static inline void	init_light_obj_params(\
t_object *obj, t_light *l, uint32_t mat_id);

t_error	init_point_light(t_context *ctx, t_light *light, uint32_t mat_id)
{
	t_light			*l;
	t_object		obj;
	t_material		*mat;
	t_error			err;

	l = malloc(sizeof(*l));
	if (!l)
		return (E_MALLOC);
	*l = *light;
	mat = ((t_material **)ctx->scene.assets.materials.items)[mat_id];
	l->emission = mat->emission;
	l->max_radiance = MAX_RADIANCE;
	l->radius_sq = l->radius * l->radius;
	l->intensity = l->intensity * vec3_length(l->emission) * l->radius;
	init_light_obj_params(&obj, l, mat_id);
	if (vec3_length(obj.transform.pos) > 500.0f)
		return (init_dir_light(ctx, l, &obj));
	err = add_object(ctx, &obj);
	if (err != E_OK)
	{
		free(l);
		return (err);
	}
	l->obj = (t_object *)vector_getlast(&ctx->scene.geo.objs);
	l->obj->flags |= OBJ_NO_CAST_SHADOW | MAT_NO_REC_SHADOW;
	if (!vector_add(&ctx->scene.env.lights, l))
	{
		free(l);
		return (E_MALLOC);
	}
	l->idx = ctx->scene.env.lights.total;
	return (E_OK);
}

static inline void	init_light_obj_params(\
t_object *obj, t_light *l, uint32_t mat_id)
{
	*obj = (t_object){0};
	obj->type = OBJ_SPHERE;
	obj->transform.pos = l->pos;
	obj->shape.sphere.radius = l->radius;
	obj->shape.sphere.radius_sq = l->radius_sq;
	obj->material_id = mat_id;
}

static inline t_error	init_dir_light(\
t_context *ctx, t_light *light, t_object *obj)
{
	t_object	*new_obj;

	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
	{
		free(light);
		return (E_MALLOC);
	}
	obj->transform.rot.w = 1.0f;
	obj->transform.scale = vec3_n(1.0f);
	update_transform(&obj->transform);
	*new_obj = *obj;
	new_obj->mat = \
((t_material **)ctx->scene.assets.materials.items)[obj->material_id];
	light->obj = new_obj;
	light->intensity = 4000000000000.0f;
	light->max_radiance = 2.0f;
	light->obj->flags |= OBJ_NO_CAST_SHADOW | MAT_NO_REC_SHADOW;
	light->idx = 0;
	ctx->scene.cam.directional_light = *light;
	ctx->scene.env.has_dir_light = true;
	free(light);
	return (E_OK);
}

void	update_lights_radii(t_context *ctx)
{
	t_light		*light;
	uint32_t	i;
	float		max_scale;

	i = 0;
	while (i < ctx->scene.env.lights.total)
	{
		light = ((t_light **)ctx->scene.env.lights.items)[i++];
		max_scale = fminf(fminf(light->obj->transform.scale.x, \
light->obj->transform.scale.y), light->obj->transform.scale.z);
		light->radius = light->obj->shape.sphere.radius * max_scale;
		light->radius_sq = light->radius * light->radius;
	}
}
