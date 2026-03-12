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
	t_vec3	p;

	if (hit->obj->type == OBJ_QUAD)
	{
		p.x = (hit->uv.x - 0.5f) * vec3_length(hit->obj->shape.quad.u);
		p.y = (hit->uv.y - 0.5f) * vec3_length(hit->obj->shape.quad.v);
		p.z = 0.0f;
		return (p);
	}
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	return (vec3_bias(p, vec3_negate(hit->normal)));
}

t_vec3	pattern_brick(const t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	int		y_int;

	scale = mat->pattern_scale;
	p = vec3_scale(get_local_coords(hit), scale);
	y_int = (int)floorf(p.y);
	if (y_int % 2 != 0)
	{
		p.x += 0.5f;
		p.z += 0.5f;
	}
	p.x = p.x - floorf(p.x);
	p.y = p.y - floorf(p.y);
	p.z = p.z - floorf(p.z);
	if (p.x < 0.05f || p.y < 0.05f || p.z < 0.05f)
		return (mat->albedo2);
	return (mat->albedo);
}
