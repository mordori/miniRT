/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

/*
** Initializes a cylinder object and adds it to the scene.
**
** @param ctx    The application context.
** @param cyl    The cylinder geometry.
** @param mat_id The material ID.
** @return       E_OK on success, error code otherwise.
*/
t_error init_cylinder(t_context* ctx, t_cylinder* cyl, int32_t mat_id) {
	t_object obj = (t_object){ 0 };
	obj.type = OBJ_CYLINDER;
	obj.material_id = mat_id;
	obj.transform.pos = cyl->center;
	obj.shape.cylinder = *cyl;
	obj.transform.rot = quat_from_dir(cyl->axis);
	cyl->center = g_zero;
	cyl->axis = g_up;
	obj.shape.cylinder = *cyl;
	return add_object(ctx, &obj);
}

/*
** Tests for an intersection between a ray and the cylinder's curved body.
**
** @param cyl The cylinder to test against.
** @param ray The incoming ray.
** @param hit Record to store hit information.
** @return    true if the ray hits the body.
*/
static bool hit_body(const t_cylinder* cyl, const t_ray* ray, t_hit* hit) {
	float t_vals[2];
	float params[2];

	if (!solve_body_quadratic(cyl, ray, t_vals))
		return false;

	params[0] = cyl->height * 0.5f;
	params[1] = hit->t;
	if (test_body_hit(ray, params, t_vals[0]) || test_body_hit(ray, params, t_vals[1])) {
		hit->t = params[1];
		return true;
	}
	return false;
}

/*
** Calculates the outward normal and UV coordinates for a body hit.
**
** @param cyl The cylinder that was hit.
** @param ray The ray that caused the intersection.
** @param hit The hit record to populate.
*/
static void compute_body_normal(const t_cylinder* cyl, const t_ray* ray, t_hit* hit) {
	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	hit->uv.u = fast_atan2f(hit->point.x, hit->point.z) * M_1_2PIf + 0.5f;
	hit->uv.v = (hit->point.y / cyl->height) + 0.5f;
	hit->normal = vec3_normalize(vec3(hit->point.x, 0.0f, hit->point.z));
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);

	hit->tangent = vec3(-hit->point.z, 0.0f, hit->point.x);
	float len_sq = hit->tangent.x * hit->tangent.x + hit->tangent.z * hit->tangent.z;
	if (len_sq < G_EPSILON)
		hit->tangent = g_right;
	else
		hit->tangent = vec3_scale(hit->tangent, 1.0f / sqrtf(len_sq));

	hit->bitangent = vec3_cross(hit->normal, hit->tangent);
}

/*
** Tests intersection with a circular flat end cap of the cylinder.
**
** @param cyl The cylinder to test.
** @param ray The incoming ray.
** @param o   Distance from center to the cap.
** @param hit The output hit record.
** @return    true if the ray hits the bounded cap.
*/
static bool hit_cap(const t_cylinder* cyl, const t_ray* ray, float o, t_hit* hit) {
	float denom = vec3_dot(ray->dir, cyl->axis);
	if (fabsf(denom) < G_EPSILON)
		return false;

	t_vec3 c_pos = vec3_scale(cyl->axis, o);
	float t = vec3_dot(vec3_sub(c_pos, ray->origin), cyl->axis) / denom;
	if (t < G_EPSILON || t >= hit->t)
		return false;

	t_vec3 pt = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	c_pos = vec3_sub(pt, c_pos);
	if (vec3_dot(c_pos, c_pos) > cyl->radius * cyl->radius)
		return false;

	hit->t = t;
	hit->point = pt;
	hit->normal = cyl->axis;
	if (o <= 0.0f)
		hit->normal = vec3_scale(cyl->axis, -1.0f);
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	compute_cap_uv(cyl, c_pos, hit);
	return true;
}

/*
** Evaluates a ray intersection with the entire cylinder (body + caps).
**
** @param shape Union holding the cylinder's geometry.
** @param ray   The incoming ray.
** @param hit   Output record storing hit data.
** @return      true if the cylinder was hit.
*/
bool hit_cylinder(const t_shape* shape, const t_ray* ray, t_hit* hit) {
	const t_cylinder* cyl = &shape->cylinder;
	if (cyl->radius < G_EPSILON || cyl->height < G_EPSILON)
		return false;

	float half_height = cyl->height * 0.5f;
	bool found = hit_body(cyl, ray, hit);
	if (found)
		compute_body_normal(cyl, ray, hit);
	found |= hit_cap(cyl, ray, half_height, hit);
	found |= hit_cap(cyl, ray, -half_height, hit);
	return found;
}

/*
** Computes planar 2D UV mapping coordinates for the circular end caps.
**
** @param cyl    The cylinder.
** @param to_hit Vector from cap center to hit.
** @param hit    The hit record to store UV data.
*/
void compute_cap_uv(const t_cylinder* cyl, t_vec3 to_hit, t_hit* hit) {
	hit->uv.u = to_hit.x / cyl->radius * 0.5f + 0.5f;
	hit->uv.v = to_hit.z / cyl->radius * 0.5f + 0.5f;
	hit->tangent = g_right;
	hit->bitangent = vec3_cross(hit->normal, hit->tangent);
}

/*
** Computes the quadratic equation roots for ray-cylinder body intersection.
** Projects the ray onto the plane perpendicular to the axis.
**
** @param cyl    The cylinder to test.
** @param ray    The incoming ray.
** @param oc     Vector from center to ray origin.
** @param t_vals Output intersection hit distances.
** @return       true if an intersection point is found.
*/
bool solve_body_quadratic(const t_cylinder* cyl, const t_ray* ray, float t_vals[2]) {
	double a = (double)ray->dir.x * ray->dir.x + (double)ray->dir.z * ray->dir.z;
	if (a < 1e-6f)
		return false;

	double half_b = (double)ray->origin.x * ray->dir.x + (double)ray->origin.z * ray->dir.z;
	double c = (double)ray->origin.x * ray->origin.x + (double)ray->origin.z * ray->origin.z - (double)cyl->radius * cyl->radius;
	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrt(discriminant);
	t_vals[0] = (float)(-half_b - discriminant) / a;
	t_vals[1] = (float)(-half_b + discriminant) / a;
	return true;
}

/*
** Validates if a ray-cylinder intersection lies within its height bounds.
**
** @param cyl    The cylinder being checked.
** @param oc     Vector from center to ray origin.
** @param ray    The incoming ray.
** @param params [0]: half_height, [1]: closest hit distance.
** @param t      Distance to evaluate.
** @return       true if valid intersection within height.
*/
bool test_body_hit(const t_ray* ray, float params[2], float t) {
	if (t <= G_EPSILON || t >= params[1])
		return false;

	float hit_y = ray->origin.y + ray->dir.y * t;
	if (hit_y < -params[0] || hit_y > params[0])
		return false;

	params[1] = t;
	return true;
}
