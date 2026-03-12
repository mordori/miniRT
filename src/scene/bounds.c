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

/*
** Compute AABB for an arbitrarily oriented cylinder.
**
** For a cylinder with normalized axis A, radius r, height h, the
** half-extent along world axis i is:
**   r * sqrt(1 - A_i^2)  +  (h/2) * |A_i|
** The first term is the max radial projection (sin of angle to world axis),
** the second is the axial projection along that world axis.
*/
t_aabb	cylinder_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_cylinder	cyl;
	t_vec3		half;
	t_vec3		a2;

	cyl = obj->shape.cylinder;
	a2.x = cyl.axis.x * cyl.axis.x;
	a2.y = cyl.axis.y * cyl.axis.y;
	a2.z = cyl.axis.z * cyl.axis.z;
	half.x = cyl.radius * sqrtf(fmaxf(0.0f, 1.0f - a2.x))
		+ (cyl.height * 0.5f) * fabsf(cyl.axis.x);
	half.y = cyl.radius * sqrtf(fmaxf(0.0f, 1.0f - a2.y))
		+ (cyl.height * 0.5f) * fabsf(cyl.axis.y);
	half.z = cyl.radius * sqrtf(fmaxf(0.0f, 1.0f - a2.z))
		+ (cyl.height * 0.5f) * fabsf(cyl.axis.z);
	aabb.min = vec3_sub(cyl.center, half);
	aabb.max = vec3_add(cyl.center, half);
	return (aabb);
}

t_aabb	cone_bounds(const t_object *obj)
{
	t_aabb		aabb;
	t_cone		cone;
	t_vec3		base_center;
	t_vec3		radial;
	t_vec3		a2;

	cone = obj->shape.cone;
	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
	a2.x = cone.axis.x * cone.axis.x;
	a2.y = cone.axis.y * cone.axis.y;
	a2.z = cone.axis.z * cone.axis.z;
	radial.x = cone.base_radius * sqrtf(fmaxf(0.0f, 1.0f - a2.x));
	radial.y = cone.base_radius * sqrtf(fmaxf(0.0f, 1.0f - a2.y));
	radial.z = cone.base_radius * sqrtf(fmaxf(0.0f, 1.0f - a2.z));
	aabb.min.v = _mm_min_ps(cone.apex.v, vec3_sub(base_center, radial).v);
	aabb.max.v = _mm_max_ps(cone.apex.v, vec3_add(base_center, radial).v);
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
