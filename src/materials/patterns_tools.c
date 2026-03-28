/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patterns_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:50:59 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:51:00 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_math.h"
#include "materials.h"

t_vec3	eval_pattern(t_material *mat, t_hit *hit)
{
	if (mat->pattern == PAT_NONE)
		return (mat->albedo);
	if (mat->pattern == PAT_CHECKERBOARD)
		return (pattern_checkerboard(hit, mat));
	if (mat->pattern == PAT_GRADIENT)
		return (pattern_gradient(hit, mat));
	if (mat->pattern == PAT_STRIPES)
		return (pattern_stripe(hit, mat));
	if (mat->pattern == PAT_SPIRAL)
		return (pattern_spiral(hit, mat));
	if (mat->pattern == PAT_GRID)
		return (pattern_grid(hit, mat));
	if (mat->pattern == PAT_BRICK)
		return (pattern_brick(hit, mat));
	if (mat->pattern == PAT_MARBLE)
		return (pattern_perlin_marble(hit, mat));
	if (mat->pattern == PAT_WOOD)
		return (pattern_perlin_wood(hit, mat));
	if (mat->pattern == PAT_TURBULENCE)
		return (pattern_perlin_turb(hit, mat));
	return (mat->albedo);
}

t_vec3	get_local_coords(const t_hit *hit)
{
	return (mat4_mul_vec3(&hit->obj->transform.world_to_object, hit->point));
}

t_vec3	pattern_brick(const t_hit *hit, const t_material *mat)
{
	float	scale;
	float	u;
	float	v;
	int		row;

	scale = mat->pattern_scale;
	u = hit->uv.u * scale;
	v = hit->uv.v * scale * 2.0f;
	row = (int)floorf(v);
	if (row % 2 != 0)
		u += 0.5f;
	u = u - floorf(u);
	v = v - floorf(v);
	if (u < 0.05f || v < 0.05f)
		return (mat->albedo2);
	return (mat->albedo);
}
