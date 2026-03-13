/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 17:53:04 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/13 17:54:36 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "materials.h"

/*
** Computes planar 2D UV mapping coordinates for the circular end caps.
**
** @param cyl    The cylinder.
** @param to_hit Vector from cap center to hit.
** @param hit    The hit record to store UV data.
*/
void	compute_cap_uv(const t_cylinder *cyl, t_vec3 to_hit, t_hit *hit)
{
	t_vec3	tang;
	t_vec3	btan;

	onb(cyl->axis, &tang, &btan);
	hit->uv.u = vec3_dot(to_hit, tang) / cyl->radius * 0.5f + 0.5f;
	hit->uv.v = vec3_dot(to_hit, btan) / cyl->radius * 0.5f + 0.5f;
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
bool	solve_body_quadratic(const t_cylinder *cyl, const t_ray *ray,
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
static float	get_axis_projection(t_cylinder *cyl, t_vec3 oc,
					t_vec3 ray_dir, float t)
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
bool	test_body_hit(t_cylinder *cyl, t_ray *ray
				, float params[2], float t)
{
	float	projection;
	t_vec3	oc;

	if (t <= G_EPSILON || t >= params[1])
		return (false);
	oc = vec3_sub(ray->origin, cyl->center);
	projection = get_axis_projection(cyl, oc, ray->dir, t);
	if (projection < -params[0] || projection > params[0])
		return (false);
	params[1] = t;
	return (true);
}
