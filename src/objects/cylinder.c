/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:53:17 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/13 17:54:59 by wshoweky         ###   ########.fr       */
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
	if (test_body_hit((t_cylinder *)cyl, (t_ray *)ray, params, t_vals[0]))
		return (hit->t = params[1], true);
	if (test_body_hit((t_cylinder *)cyl, (t_ray *)ray, params, t_vals[1]))
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
** Tests intersection with a circular flat end cap of the cylinder.
**
** @param cyl The cylinder to test.
** @param ray The incoming ray.
** @param o   Distance from center to the cap.
** @param hit The output hit record.
** @return    true if the ray hits the bounded cap.
*/
static bool	hit_cap(const t_cylinder *cyl, const t_ray *ray
				, float o, t_hit *hit)
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
