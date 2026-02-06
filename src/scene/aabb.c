#include "scene.h"
#include "libft_list.h"

static inline t_aabb	combine_aabb(const t_aabb *a, const t_aabb *b);

t_aabb	get_volume_bounds(t_object **objs, size_t n)
{
	t_aabb		res;
	t_aabb		aabb;
	size_t		i;

	res = get_object_bounds(objs[0]);
	objs[0]->bounds_center = vec3_div(vec3_add(res.min, res.max), 2.0f);
	i = 1;
	while (i < n)
	{
		aabb = get_object_bounds(objs[i]);
		objs[i]->bounds_center = vec3_div(vec3_add(aabb.min, aabb.max), 2.0f);
		res = combine_aabb(&res, &aabb);
		++i;
	}
	return (res);
}

t_aabb	get_object_bounds(const t_object *obj)
{
	t_aabb res;

	if (obj->type == OBJ_SPHERE)
		res = sphere_bounds(obj);
	else if (obj->type == OBJ_PLANE)
		res = plane_bounds(obj);
	else
		res = cylinder_bounds(obj);
	return(res);
}

static inline t_aabb	combine_aabb(const t_aabb *a, const t_aabb *b)
{
	t_aabb		res;

	res.min.v = _mm_min_ps(a->min.v, b->min.v);
	res.max.v = _mm_max_ps(a->max.v, b->max.v);
	return (res);
}

bool	hit_aabb(const t_aabb *aabb, const t_ray *ray, float closest_t, float enter)
{
	t_v4sf		t1;
	t_v4sf		t2;
	t_v4sf		near;
	t_v4sf		far;
	float		exit;

	t1 = (aabb->min.v - ray->origin.v) * ray->inv_dir.v;
	t2 = (aabb->max.v - ray->origin.v) * ray->inv_dir.v;
	near = _mm_min_ps(t1, t2);
	far = _mm_max_ps(t1, t2);
	enter = fmaxf(fmaxf(near[0], near[1]), fmaxf(near[2], 0.0f));
	exit = fminf(fminf(far[0], far[1]), fminf(far[2], closest_t));
	return (enter < exit);
}

