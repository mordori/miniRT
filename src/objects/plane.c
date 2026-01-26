#include "objects.h"
#include "materials.h"

t_error init_plane(t_context *ctx, t_vec3 point, t_vec3 normal, t_material *mat)
{
	t_object obj;

	if (!ctx || !mat)
		return (PARSE_ERR_MALLOC);
	obj = (t_object){0};
	obj.type = OBJ_PLANE;
	obj.transform.pos = point;
	obj.shape.plane.point = point;
	obj.shape.plane.normal = normal;
	obj.flags |= OBJ_CAST_SHADOWS | OBJ_VISIBLE;
	obj.material_id = new_material(ctx, mat);
	return (add_object(ctx, &obj));
}


/*
** Ray-Plane Intersection Algorithm
**
** Mathematical formula:
**   Ray:   P(t) = origin + t * direction
**   Plane: dot(P - point, normal) = 0
**
** Solving for t:
**   dot((origin + t * dir) - point, normal) = 0
**   dot(origin - point, normal) + t * dot(dir, normal) = 0
**   t = dot(point - origin, normal) / dot(dir, normal)
**
** Steps:
**   1. Check if ray is parallel to plane (denom ≈ 0)
**   2. Calculate intersection distance t
**   3. Validate t (not behind ray, closer than current hit)
**   4. Calculate intersection point
**   5. Flip normal if ray hits from behind (backface culling)
*/
bool	hit_plane(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_plane	plane;
	float	denom;
	float	t;
	t_vec3	p0_l0;

	if (!shape || !ray || !hit)
		return (false);
	plane = shape->plane;
	// Denominator: N·D (normal dot direction)
	// If ≈0, ray is parallel to plane (no intersection)
	denom = vec3_dot(plane.normal, ray->dir);
	if (fabsf(denom) < 1e-6f)
		return (false);
	// Vector from ray origin to plane point: P0 - L0
	p0_l0 = vec3_sub(plane.point, ray->origin);
	// Calculate distance along ray: t = (P0 - L0)·N / (D·N)
	t = vec3_dot(p0_l0, plane.normal) / denom;
	// Reject if: behind ray (t < epsilon) OR farther than current hit
	if (t < 1e-3f || t >= hit->t)
		return (false);
	// Valid hit: store distance and calculate intersection point
	hit->t = t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	hit->normal = plane.normal;
	// Flip normal if ray hits from behind (denom > 0 means opposite direction)
	// This makes the plane double-sided for lighting calculations
	if (denom > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	return (true);
}

t_vec3	random_point_on_plane(const t_shape *shape, float u, float v)
{
	t_vec3	result;

	(void)shape;
	(void)u;
	(void)v;
	result = vec3_n(1.0f);
	return (result);
}

t_vec3	normal_at_plane(const t_shape *shape, const t_vec3 pos)
{
	t_vec3	result;

	(void)shape;
	(void)pos;
	result = vec3_n(1.0f);
	return (result);
}
