#include "objects.h"
#include "materials.h"
#include <math.h>

/*
** Initialize a cylinder object and add it to the scene.
**
** @param ctx     - The application context containing scene data
** @param cyl     - Cylinder geometry (center, axis, radius, height)
** @param mat_id  - Index of the material in the scene materials array
** @return        - E_OK on success, error code on failure
*/
t_error	init_cylinder(t_context *ctx, t_cylinder *cyl, int32_t mat_id)
{
	t_object	obj;

	obj = (t_object){0};
	obj.type = OBJ_CYLINDER;
	obj.transform.pos = cyl->center;
	obj.shape.cylinder = *cyl;
	obj.material_id = mat_id;
	return (add_object(ctx, &obj));
}

/*
** Solve the quadratic equation for ray-cylinder body intersection.
** For an infinite cylinder along axis, we project the ray onto the
** plane perpendicular to the axis and solve for circle intersection.
**
** Equation: |P - C - ((P-C)·A)A|² = r²
** Where P = ray_origin + t * ray_dir, C = center, A = axis, r = radius
**
** This gives us: a*t² + 2*b*t + c = 0
** where:
**   a = |D_perp|²         (D_perp = D - (D·A)A)
**   b = D_perp · OC_perp  (OC_perp = OC - (OC·A)A)
**   c = |OC_perp|² - r²   (OC = ray_origin - center)
**
** @param cyl      - The cylinder to test against
** @param ray      - The incoming ray
** @param oc       - Vector from cylinder center to ray origin
** @param t_vals   - Output array for the two intersection t values
** @return         - true if intersections exist, false otherwise
*/
static bool	solve_body_quadratic(const t_cylinder *cyl, const t_ray *ray,
				const t_vec3 oc, float t_vals[2])
{
	t_vec3	dir_perp;
	t_vec3	oc_perp;
	float	a;
	float	half_b;
	float	discriminant;

	dir_perp = vec3_sub(ray->dir, vec3_scale(cyl->axis,
				vec3_dot(ray->dir, cyl->axis)));
	oc_perp = vec3_sub(oc, vec3_scale(cyl->axis, vec3_dot(oc, cyl->axis)));
	a = vec3_dot(dir_perp, dir_perp);
	if (a < G_EPSILON)
		return (false);
	half_b = vec3_dot(oc_perp, dir_perp);
	discriminant = half_b * half_b - a * (vec3_dot(oc_perp, oc_perp)
			- cyl->radius * cyl->radius);
	if (discriminant < 0.0f)
		return (false);
	discriminant = sqrtf(discriminant);
	t_vals[0] = (-half_b - discriminant) / a;
	t_vals[1] = (-half_b + discriminant) / a;
	return (true);
}

/*
** Calculate projection of hit point onto the cylinder axis.
** Used to verify that the hit point falls within the finite height.
**
** @param cyl      - The cylinder containing axis information
** @param oc       - Vector from cylinder center to ray origin
** @param ray_dir  - Direction of the ray
** @param t        - Distance along ray to the hit point
** @return         - Signed distance from center along axis
*/
static float	get_axis_projection(const t_cylinder *cyl, const t_vec3 oc,
					const t_vec3 ray_dir, float t)
{
	t_vec3	hit_vec;

	hit_vec = vec3_add(oc, vec3_scale(ray_dir, t));
	return (vec3_dot(hit_vec, cyl->axis));
}

/*
** Test a single t value for valid body intersection.
** Checks if t is positive and within the cylinder's height bounds.
**
** @param cyl      - The cylinder to test against
** @param oc       - Vector from cylinder center to ray origin
** @param ray      - The incoming ray
** @param params   - [0]: half_height, [1]: current closest t
** @param t        - The t value to test
** @return         - true if valid intersection, false otherwise
*/
static bool	test_body_hit(const t_cylinder *cyl, const t_vec3 oc,
				const t_ray *ray, float params[2], float t)
{
	float	projection;

	if (t <= G_EPSILON || t >= params[1])
		return (false);
	projection = get_axis_projection(cyl, oc, ray->dir, t);
	if (projection < -params[0] || projection > params[0])
		return (false);
	params[1] = t;
	return (true);
}

/*
** Test ray intersection with the curved body of the cylinder.
** Solves the quadratic equation and validates both potential hits.
**
** @param cyl       - The cylinder to test against
** @param ray       - The incoming ray
** @param hit       - Hit record to update if intersection found
** @return          - true if ray hits the cylinder body, false otherwise
*/
static bool	hit_body(const t_cylinder *cyl, const t_ray *ray, t_hit *hit)
{
	t_vec3	oc;
	float	t_vals[2];
	float	params[2];

	oc = vec3_sub(ray->origin, cyl->center);
	if (!solve_body_quadratic(cyl, ray, oc, t_vals))
		return (false);
	params[0] = cyl->height * 0.5f;
	params[1] = hit->t;
	if (test_body_hit(cyl, oc, ray, params, t_vals[0]))
		return (hit->t = params[1], true);
	if (test_body_hit(cyl, oc, ray, params, t_vals[1]))
		return (hit->t = params[1], true);
	return (false);
}

/*
** Calculate the normal vector for a hit on the cylinder body.
** Normal points radially outward from the cylinder axis.
**
** @param cyl       - The cylinder containing geometry data
** @param ray       - The ray that hit the cylinder
** @param hit       - Hit record to populate with normal
*/
static void	compute_body_normal(const t_cylinder *cyl, const t_ray *ray,
				t_hit *hit)
{
	t_vec3	hit_vec;
	t_vec3	axis_point;
	float	projection;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	hit_vec = vec3_sub(hit->point, cyl->center);
	projection = vec3_dot(hit_vec, cyl->axis);
	axis_point = vec3_add(cyl->center, vec3_scale(cyl->axis, projection));
	hit->normal = vec3_normalize(vec3_sub(hit->point, axis_point));
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
}

/*
** Test ray intersection with a circular end cap of the cylinder.
** Projects ray-plane intersection onto cap, validates it's within radius.
**
** @param cyl       - The cylinder containing cap geometry
** @param ray       - The incoming ray
** @param offset    - Distance from center to cap along axis (±half_height)
** @param hit       - Hit record to update if intersection found
** @return          - true if ray hits the cap, false otherwise
*/
static bool	hit_cap(const t_cylinder *cyl, const t_ray *ray,
				float offset, t_hit *hit)
{
	t_vec3	cap_center;
	float	denom;
	float	t;
	t_vec3	to_hit;

	cap_center = vec3_add(cyl->center, vec3_scale(cyl->axis, offset));
	denom = vec3_dot(ray->dir, cyl->axis);
	if (fabsf(denom) < G_EPSILON)
		return (false);
	t = vec3_dot(vec3_sub(cap_center, ray->origin), cyl->axis) / denom;
	if (t < G_EPSILON || t >= hit->t)
		return (false);
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	to_hit = vec3_sub(hit->point, cap_center);
	if (vec3_dot(to_hit, to_hit) > cyl->radius * cyl->radius)
		return (false);
	hit->t = t;
	if (offset > 0.0f)
		hit->normal = cyl->axis;
	else
		hit->normal = vec3_scale(cyl->axis, -1.0f);
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	return (true);
}

/*
** Main cylinder intersection test.
** Tests ray against both the curved body and the two circular caps.
** Returns the closest valid intersection.
**
** @param shape     - Union containing the cylinder geometry
** @param ray       - The incoming ray to test
** @param hit       - Hit record to populate with intersection data
** @return          - true if ray hits cylinder, false otherwise
*/
bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	const t_cylinder	*cyl;
	float				half_height;
	t_hit				temp_hit;
	bool				found;

	cyl = &shape->cylinder;
	if (cyl->radius < G_EPSILON || cyl->height < G_EPSILON)
		return (false);
	half_height = cyl->height * 0.5f;
	found = false;
	temp_hit = *hit;
	if (hit_body(cyl, ray, &temp_hit))
	{
		compute_body_normal(cyl, ray, &temp_hit);
		*hit = temp_hit;
		found = true;
	}
	if (hit_cap(cyl, ray, half_height, &temp_hit) && temp_hit.t < hit->t)
		return (*hit = temp_hit, true);
	if (hit_cap(cyl, ray, -half_height, &temp_hit) && temp_hit.t < hit->t)
		return (*hit = temp_hit, true);
	return (found);
}
