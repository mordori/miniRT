#include "scene.h"

t_aabb	plane_bounds(const t_object *obj)
{
	t_aabb	aabb;
	t_plane	plane;

	plane = obj->shape.plane;
	aabb.min = vec3_sub(obj->transform.pos, plane.dimensions);
	aabb.max = vec3_add(obj->transform.pos, plane.dimensions);
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

t_aabb	cylinder_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_cylinder	cylinder;
	t_vec3		r;

	cylinder = obj->shape.cylinder;
	r = (t_vec3){{cylinder.radius, 0.0f, cylinder.radius}};
	aabb.min = vec3_sub(obj->transform.pos, r);
	r = (t_vec3){{cylinder.radius, cylinder.height, cylinder.radius}};
	aabb.max = vec3_add(obj->transform.pos, r);
	return (aabb);
}
