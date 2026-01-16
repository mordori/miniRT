#include "lights.h"
#include "utils.h"


#include "objects.h"
#include "materials.h"

void	init_point_light(t_context *ctx, t_light *light)
{
	t_light	*l;

	l = malloc(sizeof(*l));
	if (!l)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	*l = *light;
	if (l->intensity < 1.01f)
		l->intensity *= 100.0f;
	l->emission = vec3_scale(l->color, l->intensity);
	l->type = LIGHT_POINT;
	vector_try_add(ctx, &ctx->scene.lights, l);

	t_object	obj;
	t_material	mat;
	float		radius;

	t_light	*l2;
	l2 = malloc(sizeof(*l2));
	if (!l2)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	l2->pos = (t_vec3){{0.0f, 0.4f, 1.3f}};
	l2->emission = vec3_scale((t_vec3){{0.1f, 0.5f, 1.05f}}, 2.0f);
	l2->type = LIGHT_AREA;

	obj = (t_object){0};

	radius = 0.3f;
	obj.type = OBJ_SPHERE;
	obj.transform.pos = l2->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_squared = radius * radius;
	mat.emission = l2->emission;
	mat.is_emissive = true;
	obj.material_id = new_material(ctx, &mat);

	// obj.flags |=  OBJ_VISIBLE;
	add_object(ctx, &obj);
	l2->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	vector_try_add(ctx, &ctx->scene.lights, l2);


	// t_object	obj2;
	// t_material	mat2;

	// t_light	*l3;
	// l3 = malloc(sizeof(*l3));
	// if (!l3)
	// 	fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	// l3->pos = (t_vec3){{4.5f, 3.0f, 3.2f}};
	// l3->emission = vec3_scale((t_vec3){{1.0f, 1.0f, 1.0f}}, 50.0f);
	// l3->type = LIGHT_AREA;

	// obj2 = (t_object){0};

	// radius = 0.35f;
	// obj2.type = OBJ_SPHERE;
	// obj2.transform.pos = l3->pos;
	// obj2.shape.sphere.center = obj2.transform.pos;
	// obj2.shape.sphere.radius = radius;
	// obj2.shape.sphere.radius_squared = radius * radius;
	// mat2.emission = l3->emission;
	// mat2.is_emissive = true;
	// obj2.material_id = new_material(ctx, &mat2);

	// // obj2.flags |= OBJ_VISIBLE;
	// add_object(ctx, &obj2);
	// l3->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	// vector_try_add(ctx, &ctx->scene.lights, l3);


	l2 = malloc(sizeof(*l2));
	if (!l2)
		fatal_error(ctx, errors(ERR_POINTLADD), __FILE__, __LINE__);
	l2->pos = (t_vec3){{0.0f, 0.2f, 5.5f}};
	l2->emission = vec3_scale((t_vec3){{1.0f, 0.05f, 0.05f}}, 100.0f);
	l2->type = LIGHT_AREA;

	obj = (t_object){0};

	radius = 0.5f;
	obj.type = OBJ_SPHERE;
	obj.transform.pos = l2->pos;
	obj.shape.sphere.center = obj.transform.pos;
	obj.shape.sphere.radius = radius;
	obj.shape.sphere.radius_squared = radius * radius;
	mat.emission = l2->emission;
	mat.is_emissive = true;
	obj.material_id = new_material(ctx, &mat);

	// obj.flags |=  OBJ_VISIBLE;
	add_object(ctx, &obj);
	l2->obj = (t_object *)vector_getlast(&ctx->scene.objs);
	vector_try_add(ctx, &ctx->scene.lights, l2);
}
