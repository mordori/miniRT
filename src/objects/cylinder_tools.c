/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 17:53:04 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/27 20:17:09 by myli-pen         ###   ########.fr       */
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
bool	solve_body_quadratic(const t_cylinder *cyl, const t_ray *ray, \
float t_vals[2])
{
	float	a;
	float	half_b;
	float	c;
	float	discriminant;

	a = ray->dir.x * ray->dir.x + ray->dir.z * ray->dir.z;
	if (a < G_EPSILON)
		return (false);
	half_b = ray->origin.x * ray->dir.x + ray->origin.z * ray->dir.z;
	c = ray->origin.x * ray->origin.x + ray->origin.z * ray->origin.z - \
cyl->radius * cyl->radius;
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0.0f)
		return (false);
	discriminant = sqrtf(discriminant);
	t_vals[0] = (-half_b - discriminant) / a;
	t_vals[1] = (-half_b + discriminant) / a;
	return (true);
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
bool	test_body_hit(const t_ray *ray, float params[2], float t)
{
	float	hit_y;

	if (t <= G_EPSILON || t >= params[1])
		return (false);
	hit_y = ray->origin.y + ray->dir.y * t;
	if (hit_y < -params[0] || hit_y > params[0])
		return (false);
	params[1] = t;
	return (true);
}
