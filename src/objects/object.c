#include "objects.h"
#include "parsing.h"
#include "utils.h"
#include "materials.h"

t_parse_error add_object(t_context *ctx, t_object *obj)
{
	t_object *new_obj;

	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
	fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*new_obj = *obj;
	vector_try_add(ctx, &ctx->scene.objs, new_obj);
	return (PARSE_OK);
}

bool	hit_object(const t_object *obj, const t_ray *ray, t_hit *hit)
{
	static const t_hit_shape	functions[] =
	{
		hit_plane,
		hit_sphere,
		hit_cylinder
	};
	bool	result;

	if (!obj)
		return (false);
	result = functions[obj->type](&obj->shape, ray, hit);
	if (result)
		hit->obj = obj;
	return (result);
}
