/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:52:56 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/31 19:08:34 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "materials.h"

t_error	init_cone(t_context *ctx, t_cone *cone, int32_t mat_id)
{
	t_object	obj;

	cone->cos_sq = cosf(cone->angle) * cosf(cone->angle);
	cone->tan_sq = tanf(cone->angle) * tanf(cone->angle);
	cone->base_radius = cone->height * tanf(cone->angle);
	cone->inv_height = 1.0f / cone->height;
	obj = (t_object){0};
	obj.type = OBJ_CONE;
	obj.material_id = mat_id;
	obj.transform.pos = cone->apex;
	obj.transform.rot = quat_from_dir(cone->axis);
	cone->apex = g_zero;
	cone->axis = g_up;
	obj.shape.cone = *cone;
	return (add_object(ctx, &obj));
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
static bool	solve_cone_quadratic(const t_cone *cone, const t_ray *ray, \
float t_vals[2])
{
	double		coef[3];
	double		discriminant;
	double		inv_a;
	float		tmp;

	compute_coefficients(cone, ray, coef);
	if (fabs(coef[0]) < 1e-6f)
		return (false);
	discriminant = coef[1] * coef[1] - coef[0] * coef[2];
	if (discriminant < 0.0f)
		return (false);
	discriminant = sqrt(discriminant);
	inv_a = 1.0f / coef[0];
	t_vals[0] = ((float)(-coef[1] - discriminant)) * inv_a;
	t_vals[1] = ((float)(-coef[1] + discriminant)) * inv_a;
	if (coef[0] < 0.0f)
	{
		tmp = t_vals[0];
		t_vals[0] = t_vals[1];
		t_vals[1] = tmp;
	}
	return (true);
}

/*
** Tests for an intersection between a ray and the curved surface of a cone.
**
** @param cone The cone to test against.
** @param ray  The incoming ray.
** @param hit  Record to store hit information.
** @return     true if the ray hits the cone's body.
*/
static bool	hit_cone_body(const t_cone *cone, const t_ray *ray, t_hit *hit)
{
	float		t_vals[2];

	if (!solve_cone_quadratic(cone, ray, t_vals))
		return (false);
	if (is_valid_body_hit(cone, ray, t_vals[0], hit->t))
		return (hit->t = t_vals[0], true);
	if (is_valid_body_hit(cone, ray, t_vals[1], hit->t))
		return (hit->t = t_vals[1], true);
	return (false);
}

/*
** Calculates the outward normal and UV coordinates for a cone body hit.
**
** @param cone The cone that was hit.
** @param ray  The ray that caused the intersection.
** @param hit  The hit record to populate.
*/
static void	compute_cone_body_normal(const t_cone *cone, const t_ray *ray,
		t_hit *hit)
{
	float		len_sq;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	hit->normal = vec3(\
hit->point.x, -hit->point.y * cone->tan_sq, hit->point.z);
	len_sq = vec3_dot(hit->normal, hit->normal);
	if (len_sq < G_EPSILON)
		hit->normal = g_up;
	else
		hit->normal = vec3_scale(hit->normal, 1.0f / sqrtf(len_sq));
	hit->uv.u = fast_atan2f(hit->point.x, hit->point.z) * M_1_2PI + 0.5f;
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
