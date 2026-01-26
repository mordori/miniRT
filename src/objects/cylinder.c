/*
** ==========================================================================
** CYLINDER RAY INTERSECTION - MATHEMATICAL EXPLANATION
** ==========================================================================
**
** A cylinder can be thought of as a circle extruded along an axis.
** We need to find where a ray intersects:
**   1. The curved body (infinite cylinder, then clamp to height)
**   2. The two flat circular caps at the top and bottom
**
** ==========================================================================
** PART 1: INFINITE CYLINDER INTERSECTION
** ==========================================================================
**
** An infinite cylinder aligned with an axis 'A' and centered at point 'C'
** with radius 'r' is defined by all points P where:
**
**     |P - C - ((P - C) · A) * A|² = r²
**
** In plain English: "The perpendicular distance from P to the axis = radius"
**
** Breaking it down:
**   - (P - C) = vector from cylinder center to point P
**   - ((P - C) · A) = how far along the axis P is (dot product projection)
**   - ((P - C) · A) * A = the component of (P-C) parallel to axis
**   - Subtracting it gives the perpendicular component
**   - Its length squared must equal r²
**
** ==========================================================================
** SUBSTITUTING THE RAY EQUATION
** ==========================================================================
**
** A ray is: P(t) = O + t * D  (Origin + t * Direction)
**
** Let: oc = O - C  (vector from cylinder center to ray origin)
**
** Substituting P(t) into the cylinder equation:
**
**     |oc + t*D - ((oc + t*D) · A) * A|² = r²
**
** Let's define perpendicular components:
**   - oc_perp = oc - (oc · A) * A    (oc perpendicular to axis)
**   - D_perp  = D  - (D · A) * A     (ray dir perpendicular to axis)
**
** The equation becomes:
**
**     |oc_perp + t * D_perp|² = r²
**
** Expanding (using |v|² = v · v):
**
**     (oc_perp + t * D_perp) · (oc_perp + t * D_perp) = r²
**
**     |D_perp|² * t²  +  2*(oc_perp · D_perp) * t  +  |oc_perp|² - r² = 0
**         ↑                      ↑                         ↑
**         a                    2*b                         c
**
** This is a quadratic equation: a*t² + 2*b*t + c = 0
**
** Solutions: t = (-b ± √(b² - ac)) / a
**
** ==========================================================================
** PART 2: HEIGHT BOUNDS
** ==========================================================================
**
** The infinite cylinder solution gives us t values, but we need to check
** if the hit point is within the finite cylinder's height.
**
** For a hit point P, project it onto the axis:
**
**     projection = (P - C) · A
**
** The point is valid if: -height/2 ≤ projection ≤ height/2
**
** ==========================================================================
** PART 3: CAP INTERSECTION
** ==========================================================================
**
** Caps are flat circles perpendicular to the axis.
** They lie on planes defined by:
**
**     Top cap:    (P - C_top) · A = 0    where C_top = C + (height/2) * A
**     Bottom cap: (P - C_bot) · A = 0    where C_bot = C - (height/2) * A
**
** For ray-plane intersection:
**
**     t = ((C_cap - O) · A) / (D · A)
**
** After finding t, check if hit point is within the cap's radius:
**
**     |P - C_cap|² ≤ r²
**
** ==========================================================================
** PART 4: NORMAL CALCULATION
** ==========================================================================
**
** Body normal: Points radially outward from the axis
**     - Find the closest point on axis to hit point
**     - Normal = normalize(hit_point - axis_point)
**
** Cap normal: Simply the axis direction (or negative for bottom cap)
**
** ==========================================================================
*/

#include "objects.h"
#include "materials.h"
#include "utils.h"
#include <math.h>
#include <stddef.h>

t_error	init_cylinder(t_context *ctx, t_cylinder *cy, t_material *mat)
{
	t_object	obj;

	obj = (t_object){0};
	obj.type = OBJ_CYLINDER;
	obj.transform.pos = cy->center;
	obj.shape.cylinder = *cy;
	obj.material_id = new_material(ctx, mat);
	obj.flags |= OBJ_CAST_SHADOWS | OBJ_VISIBLE;
	return (add_object(ctx, &obj));
}

// bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
// {
// 	t_cylinder		cylinder;
// 	float			t;

// 	(void)t;
// 	(void)cylinder;
// 	(void)ray;
// 	(void)hit;
// 	cylinder = shape->cylinder;
// 	return (true);
// }

t_vec3	random_point_on_cylinder(const t_shape *shape, float u, float v)
{
	t_vec3	result;

	(void)shape;
	(void)u;
	(void)v;
	result = vec3_n(1.0f);
	return (result);
}

t_vec3	normal_at_cylinder(const t_shape *shape, const t_vec3 pos)
{
	t_vec3	result;

	(void)shape;
	(void)pos;
	result = vec3_n(1.0f);
	return (result);
}

/*
** Solves the quadratic equation for ray-cylinder intersection.
**
** Given:
**   - cyl: cylinder with axis, center, and radius
**   - ray: ray with origin and direction
**   - oc:  vector from cylinder center to ray origin (pre-computed)
**
** Returns:
**   - true if ray intersects cylinder, with t[0] and t[1] as solutions
**   - t[0] is the closer intersection (entry point)
**   - t[1] is the farther intersection (exit point)
**
** Math recap:
**   a = |D_perp|²           (squared length of perpendicular ray direction)
**   b = oc_perp · D_perp    (using half_b to avoid multiplying by 2)
**   c = |oc_perp|² - r²     (squared distance from axis minus radius²)
**
**   discriminant = b² - ac
**   If discriminant < 0: no intersection (ray misses cylinder)
**   If discriminant ≥ 0: two solutions (ray enters and exits cylinder)
*/
static bool	solve_cylinder_quadratic(const t_cylinder *cyl, const t_ray *ray,
				t_vec3 oc, float t[2])
{
	t_vec3	oc_perp;
	t_vec3	d_perp;
	float	a;
	float	half_b;
	float	c;
	float	discriminant;

	/*
	** Project ray direction onto plane perpendicular to cylinder axis.
	** D_perp = D - (D · A) * A
	** This removes the component of D that's parallel to the axis.
	*/
	d_perp = vec3_sub(ray->dir, vec3_scale(cyl->axis, vec3_dot(ray->dir, cyl->axis)));

	/*
	** Project oc (origin-to-center) perpendicular to axis.
	** Same formula: remove the parallel component.
	*/
	oc_perp = vec3_sub(oc, vec3_scale(cyl->axis, vec3_dot(oc, cyl->axis)));

	/* a = |D_perp|² - if near zero, ray is parallel to cylinder axis */
	a = vec3_dot(d_perp, d_perp);
	if (a < G_EPSILON)
		return (false);

	/* half_b = oc_perp · D_perp (we use half to simplify the formula) */
	half_b = vec3_dot(oc_perp, d_perp);

	/* c = |oc_perp|² - r² */
	c = vec3_dot(oc_perp, oc_perp) - cyl->radius * cyl->radius;

	/* discriminant determines number of solutions */
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (false);

	/* Quadratic formula: t = (-b ± √discriminant) / a */
	t[0] = (-half_b - sqrtf(discriminant)) / a;
	t[1] = (-half_b + sqrtf(discriminant)) / a;
	return (true);
}

/*
** Check if a hit point at parameter 't' is within the cylinder's height.
**
** The cylinder extends height/2 above and below the center along the axis.
** Valid range: [-height/2, +height/2]
**
** To check:
**   1. Compute the hit point relative to center: hit_vec = oc + t * D
**   2. Project onto axis: projection = hit_vec · A
**   3. Check if projection is within bounds
**
** Visual:
**              axis
**               ↑
**      +height/2 ─────  top cap
**               |     |
**        center ─  ●  ─  (projection = 0 at center)
**               |     |
**      -height/2 ─────  bottom cap
*/
static bool	hit_within_height(const t_cylinder *cyl, t_vec3 oc,
				const t_ray *ray, float t)
{
	t_vec3	hit_vec;
	float	projection;

	/* Vector from center to hit point */
	hit_vec = vec3_add(oc, vec3_scale(ray->dir, t));

	/* How far along the axis is the hit point? */
	projection = vec3_dot(hit_vec, cyl->axis);

	/* Check bounds: must be within ±height/2 of center */
	return (projection >= -cyl->height / 2.0f && projection <= cyl->height / 2.0f);
}

/*
** Intersect ray with the curved body of the cylinder (not the caps).
**
** Algorithm:
**   1. Solve quadratic to find where ray hits infinite cylinder
**   2. Check if closer hit (t[0]) is valid (positive, closer than current, within height)
**   3. If not, try farther hit (t[1])
**   4. Compute hit point and normal
**
** Normal calculation for curved surface:
**   - Find the point on the axis closest to the hit point
**   - axis_point = center + projection * axis
**   - Normal points radially outward: normalize(hit_point - axis_point)
**
** Visual (cross-section looking down the axis):
**
**           normal
**             ↑
**         ────●────  ← hit point on surface
**        /    |    \
**       |     ○     |  ← axis_point (on center axis)
**        \         /
**         ─────────
*/
static bool	hit_cylinder_body(const t_cylinder *cyl, const t_ray *ray,
				t_vec3 center, t_hit *hit)
{
	t_vec3	oc;
	float	t[2];
	float	chosen_t;
	t_vec3	hit_vec;
	float	projection;
	t_vec3	axis_point;

	/* Vector from cylinder center to ray origin */
	oc = vec3_sub(ray->origin, center);

	/* Solve quadratic - returns two t values where ray crosses cylinder surface */
	if (!solve_cylinder_quadratic(cyl, ray, oc, t))
		return (false);

	/*
	** Try the closer intersection first (t[0]).
	** Requirements:
	**   - t > G_EPSILON (hit is in front of ray, not behind)
	**   - t < hit->t (closer than any previous hit)
	**   - within height bounds
	*/
	chosen_t = -1.0f;
	if (t[0] > G_EPSILON && t[0] < hit->t && hit_within_height(cyl, oc, ray, t[0]))
		chosen_t = t[0];
	else if (t[1] > G_EPSILON && t[1] < hit->t && hit_within_height(cyl, oc, ray, t[1]))
		chosen_t = t[1];
	if (chosen_t < 0)
		return (false);

	/* Record the hit */
	hit->t = chosen_t;
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, chosen_t));

	/*
	** Calculate surface normal:
	** 1. Get vector from center to hit point
	** 2. Project it onto axis to find closest point on axis
	** 3. Normal = direction from axis to hit point
	*/
	hit_vec = vec3_sub(hit->point, center);
	projection = vec3_dot(hit_vec, cyl->axis);
	axis_point = vec3_add(center, vec3_scale(cyl->axis, projection));
	hit->normal = vec3_normalize(vec3_sub(hit->point, axis_point));

	/*
	** Ensure normal points against ray direction (outward-facing).
	** If ray is inside cylinder, flip the normal.
	*/
	if (vec3_dot(ray->dir, hit->normal) > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	return (true);
}

/*
** Intersect ray with a single cylinder cap (flat circular end).
**
** Caps are circles lying on planes perpendicular to the cylinder axis.
**
** Algorithm:
**   1. Find where ray hits the cap's plane (ray-plane intersection)
**   2. Check if that point is within the cap's radius
**
** Ray-Plane intersection:
**   Plane equation: (P - C_cap) · A = 0  (all points on plane)
**   Substitute ray: (O + t*D - C_cap) · A = 0
**   Solve for t:    t = ((C_cap - O) · A) / (D · A)
**
**   Special case: If D · A ≈ 0, ray is parallel to cap (no hit)
**
** Parameters:
**   - offset: +height/2 for top cap, -height/2 for bottom cap
**
** Visual (side view):
**
**         ═══════════  ← top cap (offset = +height/2)
**        ║           ║
**        ║   center  ║
**        ║     ●     ║
**        ║           ║
**         ═══════════  ← bottom cap (offset = -height/2)
*/
static bool	hit_cap(const t_cylinder *cyl, const t_ray *ray, t_vec3 center,
				float offset, t_hit *hit)
{
	t_vec3	cap_center;
	float	denom;
	float	t;
	t_vec3	hit_point;
	t_vec3	to_hit;
	float	dist_sq;

	/* Cap center is offset along the axis from cylinder center */
	cap_center = vec3_add(center, vec3_scale(cyl->axis, offset));

	/*
	** denom = D · A (ray direction dotted with cap normal)
	** If ~0, ray is parallel to cap plane → no intersection
	*/
	denom = vec3_dot(ray->dir, cyl->axis);
	if (fabsf(denom) < G_EPSILON)
		return (false);

	/* t = ((C_cap - O) · A) / (D · A) */
	t = vec3_dot(vec3_sub(cap_center, ray->origin), cyl->axis) / denom;

	/* Check if hit is valid: in front of ray and closer than current best */
	if (t < G_EPSILON || t >= hit->t)
		return (false);

	/* Compute hit point and check if within cap radius */
	hit_point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	to_hit = vec3_sub(hit_point, cap_center);
	dist_sq = vec3_dot(to_hit, to_hit);

	/* If distance² > radius², point is outside the circular cap */
	if (dist_sq > cyl->radius * cyl->radius)
		return (false);

	/* Record the hit */
	hit->t = t;
	hit->point = hit_point;

	/*
	** Cap normal is simply the axis direction.
	** Top cap (+offset): normal = +axis
	** Bottom cap (-offset): normal = -axis
	*/
	if (offset > 0)
		hit->normal = cyl->axis;
	else
		hit->normal = vec3_scale(cyl->axis, -1.0f);

	/* Flip normal if ray is hitting from inside the cylinder */
	if (vec3_dot(ray->dir, hit->normal) > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	return (true);
}

/*
** Main cylinder intersection function.
**
** Tests all three parts of the cylinder and returns the closest hit:
**   1. Curved body (side surface)
**   2. Top cap (flat circle at +height/2)
**   3. Bottom cap (flat circle at -height/2)
**
** Each test updates 'hit' only if it finds a closer intersection.
** The t-value tracking ensures we always get the closest hit.
**
** Visual of complete cylinder:
**
**           ┌─────────┐  ← top cap
**          /           \
**         │             │  ← curved body
**         │      ●      │  ← center
**         │             │
**          \           /
**           └─────────┘  ← bottom cap
**               ↑
**             axis
*/
bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	t_cylinder	cyl;
	t_hit		body_hit;
	t_hit		cap_hit;
	bool		result;

	cyl = shape->cylinder;

	/* Sanity check: degenerate cylinders have no volume */
	if (cyl.radius < G_EPSILON || cyl.height < G_EPSILON)
		return (false);

	result = false;

	/* Initialize temporary hits with current best t-value */
	body_hit = *hit;
	cap_hit = *hit;

	/* Test curved body - updates hit if closer */
	if (hit_cylinder_body(&cyl, ray, cyl.center, &body_hit))
	{
		*hit = body_hit;
		result = true;
	}

	/* Test top cap (+height/2) - updates hit if closer */
	if (hit_cap(&cyl, ray, cyl.center, cyl.height / 2.0f, &cap_hit))
	{
		if (cap_hit.t < hit->t)
			*hit = cap_hit;
		result = true;
	}

	/* Test bottom cap (-height/2) - updates hit if closer */
	if (hit_cap(&cyl, ray, cyl.center, -cyl.height / 2.0f, &cap_hit))
	{
		if (cap_hit.t < hit->t)
			*hit = cap_hit;
		result = true;
	}

	return (result);
}
