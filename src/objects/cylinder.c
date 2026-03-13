/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:53:17 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:53:18 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "materials.h"
#include <math.h>

/*
** Initializes a cylinder object and adds it to the scene.
**
** @param ctx    The application context.
** @param cyl    The cylinder geometry.
** @param mat_id The material ID.
** @return       E_OK on success, error code otherwise.
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
** Computes the quadratic equation roots for ray-cylinder body intersection.
** Projects the ray onto the plane perpendicular to the axis.
**
** @param cyl    The cylinder to test.
** @param ray    The incoming ray.
** @param oc     Vector from center to ray origin.
** @param t_vals Output intersection hit distances.
** @return       true if an intersection point is found.
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
** Calculates the projection of a hit point along the cylinder's axis.
**
** @param cyl     The cylinder.
** @param oc      Vector from center to ray origin.
** @param ray_dir The ray direction.
** @param t       Distance to hit.
** @return        Signed distance from the center along the axis.
*/
static float	get_axis_projection(const t_cylinder *cyl, const t_vec3 oc,
					const t_vec3 ray_dir, float t)
{
	t_vec3	hit_vec;

	hit_vec = vec3_add(oc, vec3_scale(ray_dir, t));
	return (vec3_dot(hit_vec, cyl->axis));
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
** Tests for an intersection between a ray and the cylinder's curved body.
**
** @param cyl The cylinder to test against.
** @param ray The incoming ray.
** @param hit Record to store hit information.
** @return    true if the ray hits the body.
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
** Calculates the outward normal and UV coordinates for a body hit.
**
** @param cyl The cylinder that was hit.
** @param ray The ray that caused the intersection.
** @param hit The hit record to populate.
*/
static void	compute_body_normal(const t_cylinder *cyl, const t_ray *ray,
				t_hit *hit)
{
	t_vec3	hit_vec;
	t_vec3	axis_point;
	t_vec3	tang;
	t_vec3	btan;
	float	projection;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	hit_vec = vec3_sub(hit->point, cyl->center);
	projection = vec3_dot(hit_vec, cyl->axis);
	axis_point = vec3_add(cyl->center, vec3_scale(cyl->axis, projection));
	hit->normal = vec3_normalize(vec3_sub(hit->point, axis_point));
	onb(cyl->axis, &tang, &btan);
	hit->uv.u = fast_atan2f(vec3_dot(hit->normal, btan),
			vec3_dot(hit->normal, tang)) * M_1_2PI + 0.5f;
	hit->uv.v = (projection / cyl->height) + 0.5f;
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
}

/*
** Computes planar 2D UV mapping coordinates for the circular end caps.
**
** @param cyl    The cylinder.
** @param to_hit Vector from cap center to hit.
** @param hit    The hit record to store UV data.
*/
static void	compute_cap_uv(const t_cylinder *cyl, t_vec3 to_hit, t_hit *hit)
{
	t_vec3	tang;
	t_vec3	btan;

	onb(cyl->axis, &tang, &btan);
	hit->uv.u = vec3_dot(to_hit, tang) / cyl->radius * 0.5f + 0.5f;
	hit->uv.v = vec3_dot(to_hit, btan) / cyl->radius * 0.5f + 0.5f;
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
static bool	hit_cap(const t_cylinder *cyl, const t_ray *ray, float o, t_hit *hit)
{
	t_vec3	c_pos;
	t_vec3	pt;
	float	denom;
	float	t;

	denom = vec3_dot(ray->dir, cyl->axis);
	if (fabsf(denom) < G_EPSILON)
		return (false);
	c_pos = vec3_add(cyl->center, vec3_scale(cyl->axis, o));
	t = vec3_dot(vec3_sub(c_pos, ray->origin), cyl->axis) / denom;
	if (t < G_EPSILON || t >= hit->t)
		return (false);
	pt = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	c_pos = vec3_sub(pt, c_pos);
	if (vec3_dot(c_pos, c_pos) > cyl->radius * cyl->radius)
		return (false);
	hit->t = t;
	hit->point = pt;
	hit->normal = cyl->axis;
	if (o <= 0.0f)
		hit->normal = vec3_scale(cyl->axis, -1.0f);
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	compute_cap_uv(cyl, c_pos, hit);
	return (true);
}

/*
** Evaluates a ray intersection with the entire cylinder (body + caps).
**
** @param shape Union holding the cylinder's geometry.
** @param ray   The incoming ray.
** @param hit   Output record storing hit data.
** @return      true if the cylinder was hit.
*/
bool	hit_cylinder(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	const t_cylinder	*cyl;
	float				half_height;
	bool				found;

	cyl = &shape->cylinder;
	if (cyl->radius < G_EPSILON || cyl->height < G_EPSILON)
		return (false);
	half_height = cyl->height * 0.5f;
	found = hit_body(cyl, ray, hit);
	if (found)
		compute_body_normal(cyl, ray, hit);
	found |= hit_cap(cyl, ray, half_height, hit);
	found |= hit_cap(cyl, ray, -half_height, hit);
	return (found);
}
