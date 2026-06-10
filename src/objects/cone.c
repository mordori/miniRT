/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

t_error init_cone(t_context* ctx, t_cone* cone, int32_t mat_id) {
	cone->cos_sq = cosf(cone->angle) * cosf(cone->angle);
	cone->tan_sq = tanf(cone->angle) * tanf(cone->angle);
	cone->base_radius = cone->height * tanf(cone->angle);
	cone->inv_height = 1.0f / cone->height;

	t_object obj = (t_object){ 0 };
	obj.type = OBJ_CONE;
	obj.material_id = mat_id;
	obj.transform.pos = cone->apex;
	obj.transform.rot = quat_from_dir(cone->axis);
	cone->apex = g_zero;
	cone->axis = g_up;
	obj.shape.cone = *cone;
	return add_object(ctx, &obj, false);
}

/*
** Solves the quadratic equation for hitting the infinite double cone.
** Swaps roots if `a` is negative to ensure t_vals[0] is the closest hit.
**
** @param cone   The cone to intersect.
** @param ray    The incoming ray.
** @param oc     Vector from apex to ray origin.
** @param t_vals Output array for hit distances [near, far].
** @return       true if an intersection point is found.
*/
static bool solve_cone_quadratic(const t_cone* cone, const t_ray* ray, float t_vals[2]) {
	double coef[3];

	compute_coefficients(cone, ray, coef);
	if (fabs(coef[0]) < 1e-6f)
		return false;

	double discriminant = coef[1] * coef[1] - coef[0] * coef[2];
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrt(discriminant);
	double inv_a = 1.0f / coef[0];
	t_vals[0] = ((float)(-coef[1] - discriminant)) * inv_a;
	t_vals[1] = ((float)(-coef[1] + discriminant)) * inv_a;
	if (coef[0] < 0.0f) {
		float tmp = t_vals[0];
		t_vals[0] = t_vals[1];
		t_vals[1] = tmp;
	}
	return true;
}

/*
** Tests for an intersection between a ray and the curved surface of a cone.
**
** @param cone The cone to test against.
** @param ray  The incoming ray.
** @param hit  Record to store hit information.
** @return     true if the ray hits the cone's body.
*/
static bool hit_cone_body(const t_cone* cone, const t_ray* ray, t_hit* hit) {
	float t_vals[2];

	if (!solve_cone_quadratic(cone, ray, t_vals))
		return false;

	if (is_valid_body_hit(cone, ray, t_vals[0], hit->t)) {
		hit->t = t_vals[0];
		return true;
	}

	if (is_valid_body_hit(cone, ray, t_vals[1], hit->t)) {
		hit->t = t_vals[1];
		return true;
	}
	return false;
}

/*
** Calculates the outward normal and UV coordinates for a cone body hit.
**
** @param cone The cone that was hit.
** @param ray  The ray that caused the intersection.
** @param hit  The hit record to populate.
*/
static void compute_cone_body_normal(const t_cone* cone, const t_ray* ray, t_hit* hit) {
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	hit->normal = vec3(hit->point.x, -hit->point.y * cone->tan_sq, hit->point.z);

	float len_sq = vec3_dot(hit->normal, hit->normal);
	if (len_sq < G_EPSILON)
		hit->normal = g_up;
	else
		hit->normal = vec3_scale(hit->normal, 1.0f / sqrtf(len_sq));

	hit->uv.u = fast_atan2f(hit->point.x, hit->point.z) * M_1_2PIf + 0.5f;
	hit->uv.v = hit->point.y * cone->inv_height;

	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);

	hit->tangent = vec3(-hit->point.z, 0.0f, hit->point.x);
	len_sq = hit->tangent.x * hit->tangent.x + hit->tangent.z * hit->tangent.z;
	if (len_sq < G_EPSILON)
		hit->tangent = g_right;
	else
		hit->tangent = vec3_scale(hit->tangent, 1.0f / sqrtf(len_sq));

	hit->bitangent = vec3_cross(hit->normal, hit->tangent);
}

/*
** Evaluates a ray intersection with the entire cone (body + base cap).
** Returns the nearest valid intersection.
**
** @param shape Union holding the cone's geometry.
** @param ray   The incoming ray.
** @param hit   Output record storing hit data.
** @return      true if the cone was hit.
*/
bool hit_cone(const t_shape* shape, const t_ray* ray, t_hit* hit) {
	const t_cone* cone = &shape->cone;
	if (cone->height < G_EPSILON)
		return false;

	bool found = false;
	t_hit temp_hit = *hit;
	if (hit_cone_body(cone, ray, &temp_hit)) {
		compute_cone_body_normal(cone, ray, &temp_hit);
		*hit = temp_hit;
		found = true;
	}

	if (hit_cone_base(cone, ray, &temp_hit) && temp_hit.t < hit->t) {
		*hit = temp_hit;
		return true;
	}
	return found;
}

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
