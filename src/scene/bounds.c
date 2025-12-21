#include "scene.h"

// TODO: verify
t_aabb	plane_bounds(const t_object *obj)
{
	t_aabb		aabb;

	(void)obj;
	aabb.min = (t_vec3){{-1000, -1000, -1000}};
	aabb.max = (t_vec3){{1000, 1000, 1000}};
	return (aabb);
}

t_aabb	sphere_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_sphere	sphere;
	t_vec3		r;

	sphere = obj->shape.sphere;
	r = vec3_n(sphere.radius);
	aabb.min = vec3_sub(sphere.center, r);
	aabb.max = vec3_add(sphere.center, r);
	return (aabb);
}

// TODO: rotation
t_aabb	cylinder_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_cylinder	cylinder;
	t_vec3		half;

	cylinder = obj->shape.cylinder;
	half = (t_vec3){{cylinder.radius, cylinder.height / 2.0f, cylinder.radius}};
	aabb.min = vec3_sub(obj->transform.pos, half);
	aabb.max = vec3_add(obj->transform.pos, half);
	return (aabb);
}
