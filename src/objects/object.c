#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline t_object	init_object(char **params);
static inline void		set_transform(t_object *obj, char **params);
static inline void		set_shape(t_object *obj, char **params);

void	add_object(t_context *ctx, char **params)
{
	t_object	*obj;

	obj = malloc(sizeof(*obj));
	if (!obj)
		fatal_error(ctx, errors(ERR_OBJADD), __FILE__, __LINE__);
	*obj = init_object(params);
	vector_try_add(ctx, &ctx->scene.objs, obj);
}

static inline t_object	init_object(char **params)
{
	t_object	obj;

	(void)params;
	set_transform(&obj, params);
	set_material(&obj, params);
	set_shape(&obj, params);
	return (obj);
}

static inline void	set_transform(t_object *obj, char **params)
{
	t_transform	transform;

	transform = (t_transform){0};
	transform.scale = (t_vec3){{1.0f, 1.0f, 1.0f}};
	transform.rot = (t_vec3){{0.0f, 0.0f, 0.0f}};

	// For testing rendering
	// -----------------------
		(void)params;
		transform.pos = (t_vec3){{0.0f, 0.0f, 5.0f}};
	// -----------------------

	obj->transform = transform;
}

static inline void	set_shape(t_object *obj, char **params)
{
	t_shape	shape;

	// For testing rendering
	// -----------------------
		shape = init_sphere(params);
	// -----------------------

	obj->shape = shape;
}

bool	hit_object(t_object *obj, t_ray *ray, t_hit *hit)
{
	bool	result;

	result = hit_sphere(&obj->shape.sphere, ray, hit);

	// if (result)
		// hit->color = get_mat_color();
	return (result);
}
