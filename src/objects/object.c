#include "objects.h"
#include "parsing.h"
#include "utils.h"
#include "materials.h"

t_error add_object(t_context *ctx, t_object *obj)
{
	t_object 	*new_obj;

	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*new_obj = *obj;
	new_obj->mat = ((t_material **)ctx->scene.materials.items)[obj->material_id];
	new_obj->flags = new_obj->mat->flags;
	vector_try_add(ctx, &ctx->scene.objs, new_obj);
	return (E_OK);
}

bool	hit_object(const t_object *obj, const t_ray *ray, t_hit *hit)
{
	bool	result;

	if (!obj)
		return (false);
	if (obj->type == OBJ_SPHERE)
		result = hit_sphere(&obj->shape, ray, hit);
	else if (obj->type == OBJ_PLANE)
		result = hit_plane(&obj->shape, ray, hit);
	else if (obj->type == OBJ_CYLINDER)
		result = hit_cylinder(&obj->shape, ray, hit);
	else if (obj->type == OBJ_CONE)
		result = hit_cone(&obj->shape, ray, hit);
	else
		return (false);
	if (result)
		hit->obj = obj;
	return (result);
}

t_vec3	random_point_on_object(const t_object *obj, float u, float v)
{
	if (obj->type == OBJ_SPHERE)
		return (random_point_on_sphere(&obj->shape, u, v));
	else if (obj->type == OBJ_PLANE)
		return (random_point_on_plane(&obj->shape, u, v));
	else if (obj->type == OBJ_CYLINDER)
		return (random_point_on_cylinder(&obj->shape, u, v));
	else if (obj->type == OBJ_CONE)
		return (random_point_on_cone(&obj->shape, u, v));
	return (vec3_n(0.0f));
}

t_vec3	normal_at_point(const t_object *obj, const t_vec3 pos)
{
	if (obj->type == OBJ_SPHERE)
		return (normal_at_sphere(&obj->shape, pos));
	else if (obj->type == OBJ_PLANE)
		return (normal_at_plane(&obj->shape, pos));
	else if (obj->type == OBJ_CYLINDER)
		return (normal_at_cylinder(&obj->shape, pos));
	else if (obj->type == OBJ_CONE)
		return (normal_at_cone(&obj->shape, pos));
	return (vec3_n(0.0f));
}
