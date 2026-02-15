#include "lights.h"
#include "utils.h"
#include "objects.h"
#include "materials.h"

static inline void	init_dir_light(t_context *ctx, t_light *light, t_object *obj);

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
	obj = (t_object){0};
	obj.type = OBJ_SPHERE;
	obj.transform.pos = l->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = l->radius;
	obj.shape.sphere.radius_sq = l->radius * l->radius;
	obj.material_id = l->material_id;
	obj.flags |= OBJ_NO_CAST_SHADOW | MAT_NO_REC_SHADOW;
	if (l->radius > 20.0f)
	{
		init_dir_light(ctx, l, &obj);
		return ;
	}
	add_object(ctx, &obj);
	l->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	vector_try_add(ctx, &ctx->scene.lights, l);
}

static inline void	init_dir_light(t_context *ctx, t_light *light, t_object *obj)
{
	t_object 	*new_obj;

	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*new_obj = *obj;
	new_obj->mat = ((t_material **)ctx->scene.materials.items)[obj->material_id];
	light->obj = new_obj;
	ctx->scene.cam.directional_light = *light;
	ctx->scene.has_directional_light = true;
	free(light);
}
