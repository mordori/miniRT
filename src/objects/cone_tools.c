#include "objects.h"

/*
** Computes coefficients for the ray-cone quadratic equation.
** Uses the implicit cone equation: (V·A)² - cos²θ(V·V) = 0.
** Note: 'a' can be negative, which affects root ordering later.
**
** @param cone The cone to intersect.
** @param ray  The incoming ray.
** @param oc   Vector from apex to ray origin.
** @param coef Output array [a, half_b, c].
*/
void compute_coefficients(const t_cone* cone, const t_ray* ray, double coef[3]) {
	// clang-format off
	double d_dot_oc =	(double)ray->dir.x * ray->origin.x +
						(double)ray->dir.y * ray->origin.y +
						(double)ray->dir.z * ray->origin.z;

	double d_dot_d = 	(double)ray->dir.x * ray->dir.x +
						(double)ray->dir.y * ray->dir.y +
						(double)ray->dir.z * ray->dir.z;

	double o_dot_o = 	(double)ray->origin.x * ray->origin.x +
						(double)ray->origin.y * ray->origin.y +
						(double)ray->origin.z * ray->origin.z;
	// clang-format on
	double cos_sq = (double)cone->cos_sq;
	coef[0] = (double)ray->dir.y * ray->dir.y - cos_sq * d_dot_d;
	coef[1] = (double)ray->dir.y * ray->origin.y - cos_sq * d_dot_oc;
	coef[2] = (double)ray->origin.y * ray->origin.y - cos_sq * o_dot_o;
}

/*
** Validates whether an intersection point is within the cone's bounds.
**
** @param cone  The cone being checked.
** @param ray   The incoming ray.
** @param t     The distance to the intersection point.
** @param t_max The maximum valid hit distance.
** @return      true if the point lies on the valid part of the cone.
*/
bool is_valid_body_hit(const t_cone* cone, const t_ray* ray, float t, float t_max) {
	if (t <= G_EPSILON || t >= t_max)
		return false;

	float hit_y = ray->origin.y + ray->dir.y * t;
	if (hit_y < 0.0f || hit_y > cone->height)
		return false;
	return true;
}

/*
** Computes planar 2D UV mapping coordinates for the circular base cap.
**
** @param cone   The cone.
** @param to_hit Vector from the base center to the hit point.
** @param base_r The radius of the base circle.
** @param hit    The hit record to store UV data.
*/
void compute_cone_cap_uv(t_vec3 to_hit, float base_r, t_hit* hit) {
	hit->uv.u = to_hit.x / base_r * 0.5f + 0.5f;
	hit->uv.v = to_hit.z / base_r * 0.5f + 0.5f;
	hit->tangent = g_right;
	hit->bitangent = vec3_cross(hit->normal, hit->tangent);
}

/*
** Tests intersection with the cone's circular flat base cap.
** First intersects the infinite base plane, then bounds it by radius.
**
** @param cone The cone to test against.
** @param ray  The incoming ray.
** @param hit  The output hit record.
** @return     true if the ray hits the bounded base cap.
*/
bool hit_cone_base(const t_cone* cone, const t_ray* ray, t_hit* hit) {
	if (fabsf(ray->dir.y) < G_EPSILON)
		return false;

	float t = (cone->height - ray->origin.y) / ray->dir.y;
	if (t < G_EPSILON || t >= hit->t)
		return false;

	t_vec3 point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	t_vec3 to_hit = vec3(point.x, 0.0f, point.z);
	if (to_hit.x * to_hit.x + to_hit.z * to_hit.z > cone->base_radius * cone->base_radius)
		return false;

	hit->point = point;
	hit->t = t;
	hit->normal = g_up;
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = g_up;
	compute_cone_cap_uv(to_hit, cone->base_radius, hit);
	return true;
}
