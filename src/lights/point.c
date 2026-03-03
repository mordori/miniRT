#include "lights.h"
#include "utils.h"
#include "objects.h"
#include "materials.h"

static inline void	init_dir_light(t_context *ctx, t_light *light, t_object *obj);

void	init_point_light(t_context *ctx, t_light *light, uint32_t mat_id)
{
	t_light			*l;
	t_object		obj;
	t_material		*mat;

	l = malloc(sizeof(*l));
	if (!l)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	*l = *light;
	mat = ((t_material **)ctx->scene.assets.materials.items)[mat_id];
	l->emission = mat->emission;
	l->max_radiance = MAX_RADIANCE;
	obj = (t_object){0};
	obj.type = OBJ_SPHERE;
	// l->intensity = 20.0f;
	l->radius_sq = l->radius * l->radius;
	l->intensity = l->intensity * vec3_length(l->emission) * l->radius;
	obj.transform.pos = l->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = l->radius;
	obj.shape.sphere.radius_sq = l->radius_sq;
	obj.material_id = mat_id;
	if (vec3_length(obj.transform.pos) > 500.0f)
	{
		init_dir_light(ctx, l, &obj);
		return ;
	}
	add_object(ctx, &obj);
	l->obj = (t_object *)vector_getlast(&ctx->scene.geo.objs);
	l->obj->flags |= OBJ_NO_CAST_SHADOW | MAT_NO_REC_SHADOW;
	vector_try_add(ctx, &ctx->scene.env.lights, l);
	l->idx = ctx->scene.env.lights.total;
}

static inline void	init_dir_light(t_context *ctx, t_light *light, t_object *obj)
{
	t_object 	*new_obj;

	new_obj = malloc(sizeof(t_object));
	if (!new_obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*new_obj = *obj;
	new_obj->mat = ((t_material **)ctx->scene.assets.materials.items)[obj->material_id];
	light->obj = new_obj;
	light->intensity = 4000000000000.0f;
	light->max_radiance = 2.0f;
	light->obj->flags |= OBJ_NO_CAST_SHADOW | MAT_NO_REC_SHADOW;
	light->idx = 0;
	// light->angle = 0.53f;
	// light->pos = vec3_normalize(light->pos);
	// light->cos_theta_max = cosf(light->angle * (M_PI / 180.0f) * 0.5f);
	ctx->scene.cam.directional_light = *light;
	ctx->scene.env.has_dir_light = true;
	free(light);
}
