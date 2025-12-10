#include "scene.h"
#include "libft_list.h"

static inline bool		find_t_min_max(float *t_min, float *t_max, float min, float max);
static inline t_aabb	get_object_bounds(const t_object *obj);
static inline t_aabb	combine_aabb(const t_aabb *a, const t_aabb *b);

t_aabb	get_volume_bounds(t_object **objs, size_t n)
{
	t_aabb	result;
	t_aabb	aabb;
	size_t	i;

	result = get_object_bounds(objs[0]);
	objs[0]->bounds_center = vec3_div(vec3_add(result.min, result.max), 2.0f);
	i = 1;
	while (i < n)
	{
		aabb = get_object_bounds(objs[i]);
		objs[i]->bounds_center = vec3_div(vec3_add(aabb.min, aabb.max), 2.0f);
		result = combine_aabb(&result, &aabb);
		++i;
	}
	return (result);
}

static inline t_aabb	get_object_bounds(const t_object *obj)
{
	t_aabb	aabb;
	t_vec3	r;

	if (obj->type == OBJ_SPHERE)
	{
		r = vec3_n(obj->shape.sphere.radius);
		aabb.min = vec3_sub(obj->shape.sphere.center, r);
		aabb.max = vec3_add(obj->shape.sphere.center, r);
	}
	return (aabb);
}

static inline t_aabb	combine_aabb(const t_aabb *a, const t_aabb *b)
{
	t_aabb	result;

	result.min.x = fminf(a->min.x, b->min.x);
	result.min.y = fminf(a->min.y, b->min.y);
	result.min.z = fminf(a->min.z, b->min.z);
	result.max.x = fmaxf(a->max.x, b->max.x);
	result.max.y = fmaxf(a->max.y, b->max.y);
	result.max.z = fmaxf(a->max.z, b->max.z);
	return (result);
}

bool	hit_aabb(const t_aabb *aabb, const t_ray *ray, const float closest_t)
{
	float	t_min;
	float	t_max;
	float	min;
	float	max;

	t_min = 0.0f;
	t_max = closest_t;
	min = (aabb->min.x - ray->origin.x) * ray->inv_dir.x;
	max = (aabb->max.x - ray->origin.x) * ray->inv_dir.x;
	if (!find_t_min_max(&t_min, &t_max, min, max))
		return (false);
	min = (aabb->min.y - ray->origin.y) * ray->inv_dir.y;
	max = (aabb->max.y - ray->origin.y) * ray->inv_dir.y;
	if (!find_t_min_max(&t_min, &t_max, min, max))
		return (false);
	min = (aabb->min.z - ray->origin.z) * ray->inv_dir.z;
	max = (aabb->max.z - ray->origin.z) * ray->inv_dir.z;
	return (find_t_min_max(&t_min, &t_max, min, max));
}

static inline bool	find_t_min_max(float *t_min, float *t_max, float min, float max)
{
	*t_min = fmaxf(*t_min, fminf(min, max));
	*t_max = fminf(*t_max, fmaxf(min, max));
	return (*t_max > *t_min);
}
