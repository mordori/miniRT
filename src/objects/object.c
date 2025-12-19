#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline t_object		init_object(char **params);
static inline t_transform	set_transform(char **params);
static inline t_shape		set_shape(const t_object *obj, char **params);

void	add_object(t_context *ctx, char **params)
{
	t_object	*obj;

	obj = malloc(sizeof(*obj));
	if (!obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*obj = init_object(params);

	// For testing rendering
	// -----------------------
		obj->type = OBJ_SPHERE;
	// -----------------------

	vector_try_add(ctx, &ctx->scene.objs, obj);

	obj = malloc(sizeof(*obj));
	if (!obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*obj = init_object(params);

	// For testing rendering
	// -----------------------
		obj->type = OBJ_SPHERE;
	// -----------------------

	vector_try_add(ctx, &ctx->scene.objs, obj);
}

static inline t_object	init_object(char **params)
{
	t_object	obj;

	obj = (t_object){0};
	obj.transform = set_transform(params);
	obj.material = set_material(params);
	obj.shape = set_shape(&obj, params);
	return (obj);
}

static inline t_transform	set_transform(char **params)
{
	static int i = 0;
	t_transform	transform;

	transform.pos =  vec3_n(0.0f);
	transform.rot =  vec3_n(0.0f);
	transform.scale = vec3_n(1.0f);

	// For testing rendering
	// -----------------------
		(void)params;
		transform.pos = (t_vec3){{i * -2.2f, 0.0f, 5.0f}};
	// -----------------------
	++i;
	return (transform);
}

static inline t_shape	set_shape(const t_object *obj, char **params)
{
	t_shape	shape;

	// For testing rendering
	// -----------------------
		shape = init_sphere(obj, params);
	// -----------------------

	return (shape);
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

	result = functions[obj->type](&obj->shape, ray, hit);
	if (result)
		hit->color = obj->material.color;
	return (result);
}
