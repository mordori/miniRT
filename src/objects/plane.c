#include "objects.h"
#include "materials.h"

t_parse_error init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, t_material *mat)
{
	t_object obj;

	obj = (t_object){0};
	obj.type = OBJ_PLANE;
	obj.transform.pos = point;
	obj.shape.plane.point = point;
	obj.shape.plane.normal = normal;
	obj.material_id = new_material(ctx, mat);
	return (add_object(ctx, &obj));
}


bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_plane		plane;
	float		t;

	(void)t;
	(void)plane;
	(void)ray;
	(void)hit;
	plane = shape->plane;
	return (true);
}
