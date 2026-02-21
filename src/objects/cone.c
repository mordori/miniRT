#include "objects.h"
#include "materials.h"


t_error	init_cone(t_context *ctx, t_cone *cone, int32_t mat_id)
{
    t_object    obj;

	cone->cos_sq = cosf(cone->angle) * cosf(cone->angle);
	cone->tan_sq = tanf(cone->angle) * tanf(cone->angle);
	cone->base_radius = cone->height * tanf(cone->angle);
	cone->inv_height = 1.0f / cone->height;
    obj = (t_object){0};
    obj.type = OBJ_CONE;
    obj.shape.cone = *cone;
    obj.material_id = mat_id;
    obj.transform.pos = cone->apex;
    return (add_object(ctx, &obj));
}

/*
** Compute quadratic coefficients for ray-cone intersection.
**
** Implicit cone equation: (V·A)² - cos²θ × (V·V) = 0
** where V = P - apex, A = axis, θ = half-angle.
**
** Substituting P = O + t×D and expanding gives: a×t² + 2b×t + c = 0
**   a = (D·A)² - cos²θ × (D·D)
**   b = (D·A)(OC·A) - cos²θ × (D·OC)       [half_b]
**   c = (OC·A)² - cos²θ × (OC·OC)
**
** NOTE: Unlike cylinders where a = |D_perp|² ≥ 0, for cones `a` can be
** negative. This affects root ordering in the quadratic solver.
**
** @param cone    - The cone to test against
** @param ray     - The incoming ray
** @param oc      - Vector from apex to ray origin (O - apex)
** @param coef    - Output array [a, half_b, c]
*/
static void	compute_coefficients(const t_cone *cone, const t_ray *ray,
				const t_vec3 oc, float coef[3])
{
	float	d_dot_a;
	float	oc_dot_a;
	float	oc_dot_oc;
	float	d_dot_oc;

	d_dot_a = vec3_dot(ray->dir, cone->axis);
	oc_dot_a = vec3_dot(oc, cone->axis);
	oc_dot_oc = vec3_dot(oc, oc);
	d_dot_oc = vec3_dot(ray->dir, oc);
	coef[0] = d_dot_a * d_dot_a - cone->cos_sq;
	coef[1] = d_dot_a * oc_dot_a - cone->cos_sq * d_dot_oc;
	coef[2] = oc_dot_a * oc_dot_a - cone->cos_sq * oc_dot_oc;
}

/*
** Solve the quadratic equation for ray-cone intersection.
** Returns the two t values (entry/exit) for the infinite double cone.
**
** When a < 0, the standard formula (-b ± √disc) / a reverses root
** ordering because dividing by a negative flips the inequality.
** We swap t_vals to guarantee t_vals[0] ≤ t_vals[1] (near before far).
** Without this, hit_cone_body would return the back face → transparent.
**
** @param cone    - The cone to test against
** @param ray     - The incoming ray
** @param oc      - Vector from apex to ray origin
** @param t_vals  - Output: t_vals[0] = near hit, t_vals[1] = far hit
** @return        - true if intersections exist, false otherwise
*/
static bool	solve_cone_quadratic(const t_cone *cone, const t_ray *ray,
				const t_vec3 oc, float t_vals[2])
{
	float	coef[3];
	float	discriminant;
	float	inv_a;
	float	tmp;

	compute_coefficients(cone, ray, oc, coef);
	if (fabsf(coef[0]) < G_EPSILON)
		return (false);
	discriminant = coef[1] * coef[1] - coef[0] * coef[2];
	if (discriminant < 0.0f)
		return (false);
	discriminant = sqrtf(discriminant);
	inv_a = 1.0f / coef[0];
	t_vals[0] = (-coef[1] - discriminant) * inv_a;
	t_vals[1] = (-coef[1] + discriminant) * inv_a;
	if (coef[0] < 0.0f)
	{
		tmp = t_vals[0];
		t_vals[0] = t_vals[1];
		t_vals[1] = tmp;
	}
	return (true);
}

/*
** Check if a cone body hit is valid (within height bounds and on correct side).
** The hit must be below the apex (positive projection) and within height.
**
** @param cone    - The cone containing geometry
** @param ray     - The incoming ray
** @param t       - The t value to test
** @param t_max   - Current closest hit distance
** @return        - true if valid intersection, false otherwise
*/
static bool	is_valid_body_hit(const t_cone *cone, const t_ray *ray, float t,
				float t_max)
{
	t_vec3	hit_vec;
	float	projection;

	if (t <= G_EPSILON || t >= t_max)
		return (false);
	hit_vec = vec3_add(vec3_sub(ray->origin, cone->apex),
			vec3_scale(ray->dir, t));
	projection = vec3_dot(hit_vec, cone->axis);
	if (projection < 0.0f || projection > cone->height)
		return (false);
	return (true);
}

/*
** Test ray intersection with the curved body surface of the cone.
**
** @param cone    - The cone to test against
** @param ray     - The incoming ray
** @param hit     - Hit record to update if intersection found
** @return        - true if ray hits the cone body, false otherwise
*/
static bool	hit_cone_body(const t_cone *cone, const t_ray *ray, t_hit *hit)
{
	t_vec3	oc;
	float	t_vals[2];

	oc = vec3_sub(ray->origin, cone->apex);
	if (!solve_cone_quadratic(cone, ray, oc, t_vals))
		return (false);
	if (is_valid_body_hit(cone, ray, t_vals[0], hit->t))
		return (hit->t = t_vals[0], true);
	if (is_valid_body_hit(cone, ray, t_vals[1], hit->t))
		return (hit->t = t_vals[1], true);
	return (false);
}

/*
** Calculate the outward normal for a hit on the cone body.
**
** Formula: N = normalize(V - A × (V·A) × (1 + tan²θ))
**        = normalize(V - A × (V·A) × sec²θ)
** where V = hit_point - apex, A = axis.
**
** The factor sec²θ = 1 + tan²θ scales the axis projection to tilt
** the normal perpendicular to the cone surface. This is the gradient
** of the implicit equation (V·A)² - cos²θ(V·V) = 0, negated to
** point outward.
**
** Normal is flipped if dot(ray_dir, normal) > 0 to face the ray.
** UV: u from atan2 around the axis, v from height [0..1].
**
** @param cone    - The cone containing geometry data
** @param ray     - The ray that hit the cone
** @param hit     - Hit record to populate with point, normal, and UV
*/
static void	compute_cone_body_normal(const t_cone *cone, const t_ray *ray,
			t_hit *hit)
{
	t_vec3	apex_to_hit;
	t_vec3	axis_component;
	float	projection;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	apex_to_hit = vec3_sub(hit->point, cone->apex);
	projection = vec3_dot(apex_to_hit, cone->axis);
	axis_component = vec3_scale(cone->axis, projection * (1.0f + cone->tan_sq));
	hit->normal = vec3_normalize(vec3_sub(apex_to_hit, axis_component));
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	hit->uv.u = atan2f(hit->normal.x, hit->normal.z) * M_1_2PI + 0.5f;
	hit->uv.v = projection * cone->inv_height;
}

/*
** Test ray intersection with the circular base cap of the cone.
** The base disk is at: base_center = apex + height × axis.
** base_radius = height × tan(half_angle).
**
** Method: intersect ray with the base plane, then check if the
** hit point falls within the base circle radius.
**
** NOTE: Shadow rays from the cone body can hit this cap from inside,
** causing self-shadowing. This is handled by the skip parameter in
** hit_bvh_shadow(), not here.
**
** @param cone    - The cone containing cap geometry
** @param ray     - The incoming ray
** @param hit     - Hit record to update if intersection found
** @return        - true if ray hits the base cap, false otherwise
*/
static bool	hit_cone_base(const t_cone *cone, const t_ray *ray, t_hit *hit)
{
	t_vec3	base_center;
	float	denom;
	float	t;
	t_vec3	to_hit;
	float	base_radius;

	base_center = vec3_add(cone->apex, vec3_scale(cone->axis, cone->height));
	base_radius = cone->base_radius;
	denom = vec3_dot(ray->dir, cone->axis);
	if (fabsf(denom) < G_EPSILON)
		return (false);
	t = vec3_dot(vec3_sub(base_center, ray->origin), cone->axis) / denom;
	if (t < G_EPSILON || t >= hit->t)
		return (false);
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	to_hit = vec3_sub(hit->point, base_center);
	if (vec3_dot(to_hit, to_hit) > base_radius * base_radius)
		return (false);
	hit->t = t;
	hit->normal = vec3_scale(cone->axis, -1.0f);
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	hit->uv.u = (to_hit.x / base_radius) * 0.5f + 0.5f;
	hit->uv.v = (to_hit.z / base_radius) * 0.5f + 0.5f;
	return (true);
}

/*
** Main cone intersection test.
** Tests ray against both the curved body and the circular base cap.
** Returns the closest valid intersection.
**
** @param shape   - Union containing the cone geometry
** @param ray     - The incoming ray to test
** @param hit     - Hit record to populate with intersection data
** @return        - true if ray hits cone, false otherwise
*/
bool	hit_cone(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	const t_cone	*cone;
	t_hit			temp_hit;
	bool			found;

	cone = &shape->cone;
	if (cone->height < G_EPSILON)
		return (false);
	found = false;
	temp_hit = *hit;
	if (hit_cone_body(cone, ray, &temp_hit))
	{
		compute_cone_body_normal(cone, ray, &temp_hit);
		*hit = temp_hit;
		found = true;
	}
	if (hit_cone_base(cone, ray, &temp_hit) && temp_hit.t < hit->t)
		return (*hit = temp_hit, true);
	return (found);
}
