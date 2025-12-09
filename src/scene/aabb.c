#include "scene.h"

static inline float	get_inv_dir(float dir);
static inline void	find_t_min_max(float *t_min, float *t_max, float min, float max);

t_aabb	get_object_bounds(t_object *obj)
{
	t_aabb	box;
	t_vec3	r;

	if (obj->type == OBJ_SPHERE)
	{
		r = vec3_n(obj->shape.sphere.radius);
		box.min = vec3_sub(obj->shape.sphere.center, r);
		box.max = vec3_add(obj->shape.sphere.center, r);
	}
	return (box);
}

t_aabb	combine_boxes(t_aabb a, t_aabb b)
{
	t_aabb	result;

	result.min.x = fminf(a.min.x, b.min.x);
	result.min.y = fminf(a.min.y, b.min.y);
	result.min.z = fminf(a.min.z, b.min.z);
	result.max.x = fmaxf(a.max.x, b.max.x);
	result.max.y = fmaxf(a.max.y, b.max.y);
	result.max.z = fmaxf(a.max.z, b.max.z);
	return (result);
}

bool	hit_aabb(t_aabb *box, t_ray *ray)
{
	float	t_min;
	float	t_max;
	float	min;
	float	max;
	float	inv_dir;

	t_min = 0.0f;
	t_max = M_INF;
	inv_dir = get_inv_dir(ray->dir.x);
	min = (box->min.x - ray->origin.x) * inv_dir;
	max = (box->max.x - ray->origin.x) * inv_dir;
	find_t_min_max(&t_min, &t_max, min, max);
	inv_dir = get_inv_dir(ray->dir.y);
	min = (box->min.y - ray->origin.y) * inv_dir;
	max = (box->max.y - ray->origin.y) * inv_dir;
	find_t_min_max(&t_min, &t_max, min, max);
	inv_dir = get_inv_dir(ray->dir.z);
	min = (box->min.z - ray->origin.z) * inv_dir;
	max = (box->max.z - ray->origin.z) * inv_dir;
	find_t_min_max(&t_min, &t_max, min, max);
	return (t_max > t_min);
}

static inline float	get_inv_dir(float dir)
{
	if (fabsf(dir) < 1e-6f)
	{
		if (dir >= 0)
			return (M_INF);
		return (-M_INF);
	}
	return (1.0f / dir);
}

static inline void	find_t_min_max(float *t_min, float *t_max, float min, float max)
{
	*t_min = fmaxf(*t_min, fminf(min, max));
	*t_max = fminf(*t_max, fmaxf(min, max));
}
