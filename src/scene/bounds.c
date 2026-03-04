#include "scene.h"

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

t_aabb	cone_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_cone		cone;
	t_vec3		base_center;
	t_vec3		r;

	cone = obj->shape.cone;
	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
	r = vec3_n(cone.base_radius);
	aabb.min.v = _mm_min_ps(cone.apex.v, vec3_sub(base_center, r).v);
	aabb.max.v = _mm_max_ps(cone.apex.v, vec3_add(base_center, r).v);
	return (aabb);
}

t_aabb	quad_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_quad		quad;
	t_vec3		p1;
	t_vec3		p2;
	t_vec3		p3;

	quad = obj->shape.quad;
	p1 = vec3_add(quad.q, quad.u);
	p2 = vec3_add(quad.q, quad.v);
	p3 = vec3_add(p1, quad.v);
	aabb.min.v = _mm_min_ps(_mm_min_ps(quad.q.v, p1.v),
			_mm_min_ps(p2.v, p3.v));
	aabb.max.v = _mm_max_ps(_mm_max_ps(quad.q.v, p1.v),
			_mm_max_ps(p2.v, p3.v));
	aabb.min = vec3_sub(aabb.min, vec3_n(B_EPSILON));
	aabb.max = vec3_add(aabb.max, vec3_n(B_EPSILON));
	return (aabb);
}
