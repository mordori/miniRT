#include "lights.h"
#include "utils.h"
#include "objects.h"
#include "materials.h"

void	init_point_light(t_context *ctx, t_light *light)
{
	// t_light	*l;

	// l = malloc(sizeof(*l));
	// if (!l)
	// 	fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	// *l = *light;
	// if (l->intensity < 1.01f)
	// 	l->intensity *= 100.0f;
	// l->emission = vec3_scale(l->color, l->intensity);
	// l->type = LIGHT_POINT;
	// vector_try_add(ctx, &ctx->scene.lights, l);

	(void)ctx;
	(void)light;

	t_object	obj;
	t_material	mat;
	float		radius;
	t_light	*l2;

	l2 = malloc(sizeof(*l2));
	if (!l2)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	l2->pos = (t_vec3){{0.0f, 0.4f, 1.3f}};
	l2->emission = vec3_scale((t_vec3){{0.1f, 0.5f, 1.05f}}, 40.0f);
	l2->type = LIGHT_AREA;

	obj = (t_object){0};
	radius = 0.3f;
	obj.type = OBJ_SPHERE;
	obj.transform.pos = l2->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_sq = radius * radius;
	mat.emission = l2->emission;
	mat.is_emissive = true;
	obj.material_id = new_material(ctx, &mat);
	add_object(ctx, &obj);
	l2->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	vector_try_add(ctx, &ctx->scene.lights, l2);


	l2 = malloc(sizeof(*l2));
	if (!l2)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	l2->pos = (t_vec3){{0.0f, 0.2f, 5.5f}};
	l2->emission = vec3_scale((t_vec3){{1.0f, 0.05f, 0.05f}}, 2000.0f);
	l2->type = LIGHT_AREA;

	obj = (t_object){0};
	radius = 0.5f;
	obj.type = OBJ_SPHERE;
	obj.transform.pos = l2->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_sq = radius * radius;
	mat.emission = l2->emission;
	mat.is_emissive = true;
	obj.material_id = new_material(ctx, &mat);

	// obj.flags |=  OBJ_VISIBLE;
	add_object(ctx, &obj);
	l2->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	vector_try_add(ctx, &ctx->scene.lights, l2);
}
