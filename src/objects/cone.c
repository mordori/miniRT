#include "objects.h"
#include "materials.h"


// t_error	init_cone(t_context *ctx, t_cone *cone, int32_t mat_id)
// {
//     t_object    obj;
// 	float		cos_angle;
// 	float		sin_angle;

// 	cos_angle = cosf(cone->angle);
// 	sin_angle = sinf(cone->angle);
// 	cone->cos_sq = cos_angle * cos_angle;
// 	cone->tan_sq = (sin_angle * sin_angle) / (cos_angle * cos_angle);
//     obj = (t_object){0};
//     obj.type = OBJ_CONE;
//     obj.shape.cone = *cone;
//     obj.material_id = mat_id;
//     obj.transform.pos = cone->apex;
//     obj.flags = OBJ_VISIBLE;
//     return (add_object(ctx, &obj));
// }

// /*
// ** Compute quadratic coefficients for ray-cone intersection.
// **
// ** The cone equation is: (V·A)² - cos²θ(V·V) = 0
// ** where V = P - apex, A = axis, θ = half-angle.
// **
// ** Substituting P = O + t*D (ray equation) and expanding:
// **   a*t² + 2*b*t + c = 0
// **
// ** @param cone    - The cone to test against
// ** @param ray     - The incoming ray
// ** @param oc      - Vector from apex to ray origin (O - apex)
// ** @param coef    - Output array [a, half_b, c]
// */
// static void	compute_coefficients(const t_cone *cone, const t_ray *ray,
// 				const t_vec3 oc, float coef[3])
// {
// 	float	d_dot_a;
// 	float	oc_dot_a;
// 	float	d_dot_d;
// 	float	oc_dot_oc;
// 	float	d_dot_oc;

// 	d_dot_a = vec3_dot(ray->dir, cone->axis);
// 	oc_dot_a = vec3_dot(oc, cone->axis);
// 	d_dot_d = vec3_dot(ray->dir, ray->dir);
// 	oc_dot_oc = vec3_dot(oc, oc);
// 	d_dot_oc = vec3_dot(ray->dir, oc);
// 	coef[0] = d_dot_a * d_dot_a - cone->cos_sq * d_dot_d;
// 	coef[1] = d_dot_a * oc_dot_a - cone->cos_sq * d_dot_oc;
// 	coef[2] = oc_dot_a * oc_dot_a - cone->cos_sq * oc_dot_oc;
// }

// /*
// ** Solve the quadratic equation for ray-cone intersection.
// **
// ** @param cone    - The cone to test against
// ** @param ray     - The incoming ray
// ** @param oc      - Vector from apex to ray origin
// ** @param t_vals  - Output array for the two intersection t values
// ** @return        - true if intersections exist, false otherwise
// */
// static bool	solve_cone_quadratic(const t_cone *cone, const t_ray *ray,
// 				const t_vec3 oc, float t_vals[2])
// {
// 	float	coef[3];
// 	float	discriminant;

// 	compute_coefficients(cone, ray, oc, coef);
// 	if (fabsf(coef[0]) < G_EPSILON)
// 		return (false);
// 	discriminant = coef[1] * coef[1] - coef[0] * coef[2];
// 	if (discriminant < 0.0f)
// 		return (false);
// 	discriminant = sqrtf(discriminant);
// 	t_vals[0] = (-coef[1] - discriminant) / coef[0];
// 	t_vals[1] = (-coef[1] + discriminant) / coef[0];
// 	return (true);
// }

// /*
// ** Check if a cone body hit is valid (within height bounds and on correct side).
// ** The hit must be below the apex (positive projection) and within height.
// **
// ** @param cone    - The cone containing geometry
// ** @param oc      - Vector from apex to ray origin
// ** @param ray     - The incoming ray
// ** @param t       - The t value to test
// ** @param t_max   - Current closest hit distance
// ** @return        - true if valid intersection, false otherwise
// */
// static bool	is_valid_body_hit(const t_cone *cone, const t_vec3 oc,
// 				const t_ray *ray, float t, float t_max)
// {
// 	t_vec3	hit_vec;
// 	float	projection;

// 	if (t <= G_EPSILON || t >= t_max)
// 		return (false);
// 	hit_vec = vec3_add(oc, vec3_scale(ray->dir, t));
// 	projection = vec3_dot(hit_vec, cone->axis);
// 	if (projection < 0.0f || projection > cone->height)
// 		return (false);
// 	return (true);
// }

// /*
// ** Test ray intersection with the curved body surface of the cone.
// **
// ** @param cone    - The cone to test against
// ** @param ray     - The incoming ray
// ** @param hit     - Hit record to update if intersection found
// ** @return        - true if ray hits the cone body, false otherwise
// */
// static bool	hit_cone_body(const t_cone *cone, const t_ray *ray, t_hit *hit)
// {
// 	t_vec3	oc;
// 	float	t_vals[2];

// 	oc = vec3_sub(ray->origin, cone->apex);
// 	if (!solve_cone_quadratic(cone, ray, oc, t_vals))
// 		return (false);
// 	if (is_valid_body_hit(cone, oc, ray, t_vals[0], hit->t))
// 		return (hit->t = t_vals[0], true);
// 	if (is_valid_body_hit(cone, oc, ray, t_vals[1], hit->t))
// 		return (hit->t = t_vals[1], true);
// 	return (false);
// }

// /*
// ** Calculate the normal vector for a hit on the cone body.
// ** The normal is perpendicular to the cone surface, pointing outward.
// **
// ** For a cone, the normal at point P can be computed by:
// ** N = normalize(V - A * (|V| * cos(θ) / |V_perp| * sin(θ)))
// ** Or equivalently using the gradient of the implicit cone equation.
// **
// ** @param cone    - The cone containing geometry data
// ** @param ray     - The ray that hit the cone
// ** @param hit     - Hit record to populate with normal
// */
// static void	compute_cone_body_normal(const t_cone *cone, const t_ray *ray,
// 				t_hit *hit)
// {
// 	t_vec3	apex_to_hit;
// 	t_vec3	axis_component;
// 	float	projection;

// 	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
// 	apex_to_hit = vec3_sub(hit->point, cone->apex);
// 	projection = vec3_dot(apex_to_hit, cone->axis);
// 	axis_component = vec3_scale(cone->axis, projection * (1.0f + cone->tan_sq));
// 	hit->normal = vec3_normalize(vec3_sub(apex_to_hit, axis_component));
// 	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
// 		hit->normal = vec3_scale(hit->normal, -1.0f);
// 	hit->uv.u = atan2f(hit->normal.x, hit->normal.z) * M_1_2PI + 0.5f;
// 	hit->uv.v = projection / cone->height;
// }

// /*
// ** Test ray intersection with the circular base cap of the cone.
// ** The base is located at apex + height * axis.
// **
// ** @param cone    - The cone containing cap geometry
// ** @param ray     - The incoming ray
// ** @param hit     - Hit record to update if intersection found
// ** @return        - true if ray hits the base cap, false otherwise
// */
// static bool	hit_cone_base(const t_cone *cone, const t_ray *ray, t_hit *hit)
// {
// 	t_vec3	base_center;
// 	float	denom;
// 	float	t;
// 	t_vec3	to_hit;
// 	float	base_radius;

// 	base_center = vec3_add(cone->apex, vec3_scale(cone->axis, cone->height));
// 	base_radius = cone->height * sqrtf(cone->tan_sq);
// 	denom = vec3_dot(ray->dir, cone->axis);
// 	if (fabsf(denom) < G_EPSILON)
// 		return (false);
// 	t = vec3_dot(vec3_sub(base_center, ray->origin), cone->axis) / denom;
// 	if (t < G_EPSILON || t >= hit->t)
// 		return (false);
// 	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
// 	to_hit = vec3_sub(hit->point, base_center);
// 	if (vec3_dot(to_hit, to_hit) > base_radius * base_radius)
// 		return (false);
// 	hit->t = t;
// 	hit->normal = vec3_scale(cone->axis, -1.0f);
// 	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
// 		hit->normal = vec3_scale(hit->normal, -1.0f);
// 	return (true);
// }

// /*
// ** Main cone intersection test.
// ** Tests ray against both the curved body and the circular base cap.
// ** Returns the closest valid intersection.
// **
// ** @param shape   - Union containing the cone geometry
// ** @param ray     - The incoming ray to test
// ** @param hit     - Hit record to populate with intersection data
// ** @return        - true if ray hits cone, false otherwise
// */
// bool	hit_cone(const t_shape *shape, const t_ray *ray, t_hit *hit)
// {
// 	const t_cone	*cone;
// 	t_hit			temp_hit;
// 	bool			found;

// 	cone = &shape->cone;
// 	if (cone->height < G_EPSILON)
// 		return (false);
// 	found = false;
// 	temp_hit = *hit;
// 	if (hit_cone_body(cone, ray, &temp_hit))
// 	{
// 		compute_cone_body_normal(cone, ray, &temp_hit);
// 		*hit = temp_hit;
// 		found = true;
// 	}
// 	if (hit_cone_base(cone, ray, &temp_hit) && temp_hit.t < hit->t)
// 		return (*hit = temp_hit, true);
// 	return (found);
// }

// /*
// ** Generate a random point on the cone surface (stub implementation).
// ** Used for area light sampling and importance sampling.
// **
// ** @param shape   - Union containing the cone geometry
// ** @param u       - Random parameter in [0, 1]
// ** @param v       - Random parameter in [0, 1]
// ** @return        - A point on the cone surface
// */
// t_vec3	random_point_on_cone(const t_shape *shape, float u, float v)
// {
// 	(void)shape;
// 	(void)u;
// 	(void)v;
// 	return (vec3_n(1.0f));
// }

// /*
// ** Calculate the outward normal at a point on the cone (stub).
// ** Used when normal information is needed separately from intersection.
// **
// ** @param shape   - Union containing the cone geometry
// ** @param pos     - Point on the cone surface
// ** @return        - Unit normal vector at that point
// */
// t_vec3	normal_at_cone(const t_shape *shape, const t_vec3 pos)
// {
// 	(void)shape;
// 	(void)pos;
// 	return (vec3_n(1.0f));
// }


