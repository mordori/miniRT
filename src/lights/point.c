#include "lights.h"
#include "utils.h"
#include "objects.h"
#include "materials.h"

void	init_point_light(t_context *ctx, t_light *light)
{
	t_light			*l;
	t_object		obj;

	l = malloc(sizeof(*l));
	if (!l)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	*l = *light;
	if (l->intensity < 1.01f)
		l->intensity *= 100.0f;
	l->mat = ((t_material **)ctx->scene.materials.items)[l->material_id];
	l->type = LIGHT_POINT;


	obj = (t_object){0};
	obj.type = OBJ_SPHERE;
	obj.transform.pos = l->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = l->radius;
	obj.shape.sphere.radius_sq = l->radius * l->radius;
	obj.material_id = l->material_id;
	obj.flags |= OBJ_VISIBLE | OBJ_HIDDEN_CAM;
	add_object(ctx, &obj);
	l->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	vector_try_add(ctx, &ctx->scene.lights, l);
}
