/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 17:52:54 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/13 17:54:52 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "materials.h"

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
void	compute_coefficients(const t_cone *cone, const t_ray *ray,
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
** Validates whether an intersection point is within the cone's bounds.
**
** @param cone  The cone being checked.
** @param ray   The incoming ray.
** @param t     The distance to the intersection point.
** @param t_max The maximum valid hit distance.
** @return      true if the point lies on the valid part of the cone.
*/
bool	is_valid_body_hit(const t_cone *cone, const t_ray *ray, float t,
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
** Computes planar 2D UV mapping coordinates for the circular base cap.
**
** @param cone   The cone.
** @param to_hit Vector from the base center to the hit point.
** @param base_r The radius of the base circle.
** @param hit    The hit record to store UV data.
*/
void	compute_cone_cap_uv(const t_cone *cone, t_vec3 to_hit,
		float base_r, t_hit *hit)
{
	t_vec3	tang;
	t_vec3	btan;

	onb(cone->axis, &tang, &btan);
	hit->uv.u = vec3_dot(to_hit, tang) / base_r * 0.5f + 0.5f;
	hit->uv.v = vec3_dot(to_hit, btan) / base_r * 0.5f + 0.5f;
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
bool	hit_cone_base(const t_cone *cone, const t_ray *ray, t_hit *hit)
{
	t_vec3	base_c;
	t_vec3	to_hit;
	t_vec3	point;
	float	denom;
	float	t;

	denom = vec3_dot(ray->dir, cone->axis);
	if (fabsf(denom) < G_EPSILON)
		return (false);
	base_c = vec3_add(cone->apex, vec3_scale(cone->axis, cone->height));
	t = vec3_dot(vec3_sub(base_c, ray->origin), cone->axis) / denom;
	if (t < G_EPSILON || t >= hit->t)
		return (false);
	point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	to_hit = vec3_sub(point, base_c);
	if (vec3_dot(to_hit, to_hit) > cone->base_radius * cone->base_radius)
		return (false);
	hit->point = point;
	hit->t = t;
	hit->normal = vec3_scale(cone->axis, -1.0f);
	if (vec3_dot(ray->dir, hit->normal) > 0.0f)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	compute_cone_cap_uv(cone, to_hit, cone->base_radius, hit);
	return (true);
}
