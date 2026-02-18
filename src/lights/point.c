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
	l->mat = ((t_material **)ctx->scene.materials.items)[l->material_id];
	l->max_brightness = MAX_BRIGHTNESS;
	obj = (t_object){0};
	obj.type = OBJ_SPHERE;
	l->intensity = 10.0f;
	obj.transform.pos = l->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = l->radius;
	obj.shape.sphere.radius_sq = l->radius * l->radius;
	obj.material_id = l->material_id;
	if (l->radius > 20.0f)
	{
		l->intensity = 4000000.0f;
		l->max_brightness = 2.0f;
		init_dir_light(ctx, l, &obj);
		return ;
	}
	add_object(ctx, &obj);
	l->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	l->obj->flags |= OBJ_NO_CAST_SHADOW | MAT_NO_REC_SHADOW;
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
