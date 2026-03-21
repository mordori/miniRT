#include "scene.h"
#include <string.h>

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
	t_aabb	 res;

	memset(&res, 0, sizeof(t_aabb));
	if (obj->type == OBJ_SPHERE)
		res = sphere_bounds(obj);
	else if (obj->type == OBJ_CYLINDER)
		res = cylinder_bounds(obj);
	else if (obj->type == OBJ_CONE)
		res = cone_bounds(obj);
	else if (obj->type == OBJ_QUAD)
		res = quad_bounds(obj);
	return(res);
}

t_aabb	aabb_object_to_world(t_aabb aabb, const t_mat4 *object_to_world)
{
	t_aabb		res;
	t_vec3		corners[8];
	t_vec4		p;
	uint32_t	i;

	corners[0] = vec3(aabb.min.x, aabb.min.y, aabb.min.z);
	corners[1] = vec3(aabb.max.x, aabb.min.y, aabb.min.z);
	corners[2] = vec3(aabb.min.x, aabb.max.y, aabb.min.z);
	corners[3] = vec3(aabb.max.x, aabb.max.y, aabb.min.z);
	corners[4] = vec3(aabb.min.x, aabb.min.y, aabb.max.z);
	corners[5] = vec3(aabb.max.x, aabb.min.y, aabb.max.z);
	corners[6] = vec3(aabb.min.x, aabb.max.y, aabb.max.z);
	corners[7] = vec3(aabb.max.x, aabb.max.y, aabb.max.z);
	res.min = vec3_n(M_INF);
	res.max = vec3_n(-M_INF);
	i = 0;
	while (i < 8)
	{
		p = mat4_mul_vec4(object_to_world, vec4_3(corners[i], 1.0f));
		res.min.v = _mm_min_ps(res.min.v, p.v);
		res.max.v = _mm_max_ps(res.max.v, p.v);
		++i;
	}
	return (res);
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

	t1 = (aabb->min.v - ray->origin.v) * ray->dir_recip.v;
	t2 = (aabb->max.v - ray->origin.v) * ray->dir_recip.v;
	near = _mm_min_ps(t1, t2);
	far = _mm_max_ps(t1, t2);
	enter = fmaxf(fmaxf(near[0], near[1]), fmaxf(near[2], 0.0f));
	exit = fminf(fminf(far[0], far[1]), fminf(far[2], closest_t));
	return (enter < exit);
}
