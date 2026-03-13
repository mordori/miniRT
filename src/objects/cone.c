/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:52:56 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/13 17:54:45 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
** Solves the quadratic equation for hitting the infinite double cone.
** Swaps roots if `a` is negative to ensure t_vals[0] is the closest hit.
**
** @param cone   The cone to intersect.
** @param ray    The incoming ray.
** @param oc     Vector from apex to ray origin.
** @param t_vals Output array for hit distances [near, far].
** @return       true if an intersection point is found.
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
** Tests for an intersection between a ray and the curved surface of a cone.
**
** @param cone The cone to test against.
** @param ray  The incoming ray.
** @param hit  Record to store hit information.
** @return     true if the ray hits the cone's body.
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
** Calculates the outward normal and UV coordinates for a cone body hit.
**
** @param cone The cone that was hit.
** @param ray  The ray that caused the intersection.
** @param hit  The hit record to populate.
*/
static void	compute_cone_body_normal(const t_cone *cone, const t_ray *ray,
			t_hit *hit)
{
	t_vec3	apex_to_hit;
	t_vec3	axis_component;
	t_vec3	tang;
	t_vec3	btan;
	float	projection;

	hit->point = vec3_add(ray->origin, vec3_scale(ray->dir, hit->t));
	apex_to_hit = vec3_sub(hit->point, cone->apex);
	projection = vec3_dot(apex_to_hit, cone->axis);
	axis_component = vec3_scale(cone->axis, projection * (1.0f + cone->tan_sq));
	hit->normal = vec3_normalize(vec3_sub(apex_to_hit, axis_component));
	onb(cone->axis, &tang, &btan);
	hit->uv.u = fast_atan2f(vec3_dot(hit->normal, btan),
			vec3_dot(hit->normal, tang)) * M_1_2PI + 0.5f;
	hit->uv.v = projection * cone->inv_height;
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
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
