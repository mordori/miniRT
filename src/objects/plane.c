#include "objects.h"
#include "materials.h"
#include "utils.h"

t_error	init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, uint32_t mat_id)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*obj = (t_object){0};
	obj->type = OBJ_PLANE;
	obj->transform.pos = point;
	obj->shape.plane.point = point;
	obj->shape.plane.normal = normal;
	obj->material_id = mat_id;
	obj->mat = ((t_material **)ctx->scene.materials.items)[mat_id];
	obj->flags = obj->mat->flags;
	vector_try_add(ctx, &ctx->scene.planes, obj);
	return (E_OK);
}

bool	hit_planes(const t_context *ctx, const t_ray *ray, t_hit *hit)
{
	size_t		idx;
	bool		res;
	t_object	*obj;

	res = false;
	idx = ctx->scene.planes.total;
	while (idx--)
	{
		obj = ((t_object **)ctx->scene.planes.items)[idx];
		if (hit_plane(&obj->shape, ray, hit))
		{
			hit->obj = obj;
			res = true;
		}
	}
	return (res);
}

bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_plane		plane;
	float		denom;
	float		t;
	t_vec3		p0_l0;

	plane = shape->plane;
	denom = vec3_dot(plane.normal, ray->dir);
	if (fabsf(denom) < 1e-6f)
		return (false);
	p0_l0 = vec3_sub(plane.point, ray->origin);
	t = vec3_dot(p0_l0, plane.normal) / denom;
	if (t < 1e-3f || t >= hit->t)
		return (false);
	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	hit->normal = plane.normal;
	if (denom > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	return (true);
}
