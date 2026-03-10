/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:51:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:52:06 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "materials.h"
#include <math.h>

static inline float	object_seed(const t_object *obj)
{
	float	n;

	n = tanf(obj->transform.pos.x * 12.9898f
			+ obj->transform.pos.y * 78.233f
			+ obj->transform.pos.z * 37.719f) * 43758.5453f;
	return ((n - floorf(n)) * M_TAU);
}

/*
** Turbulence: sum of absolute values of noise at decreasing scales.
** Each octave doubles the frequency and halves the amplitude,
** creating a fractal pattern with detail at multiple scales.
**
** Used by marble, wood, and turbulence patterns.
**
** depth=7 octaves provides a good balance of detail and performance.
**
** @param p     - Point in world space to evaluate
** @return      - Turbulence value (always positive), typically [0, ~1]
*/
float	turbulence(t_vec3 p)
{
	float	accum;
	float	weight;
	int		i;

	accum = 0.0f;
	weight = 1.0f;
	i = 0;
	while (i < 7)
	{
		accum += fabsf(perlin_noise(p.x, p.y, p.z)) * weight;
		weight *= 0.5f;
		p = vec3_scale(p, 2.0f);
		++i;
	}
	return (accum);
}

/*
** Perlin marble pattern.
** Uses the classic technique from Shirley's "Ray Tracing: The Next Week":
** color = sin(scale * z + 10 * turbulence(point))
**
** The sin() creates the base veining structure, while turbulence adds
** natural-looking distortion to the veins, making them twist and fold
** like real marble.
**
** @param hit  - Hit record (point is used, bump_offset is written)
** @param mat  - Material with two albedo colors and scale/bump settings
** @return     - Blended color based on marble vein pattern
*/
t_vec3	pattern_perlin_marble(t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	float	t;
	float	seed;

	scale = mat->pattern_scale;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	p = vec3_scale(p, scale);
	seed = object_seed(hit->obj);
	p = vec3_add(p, (t_vec3){seed, seed, seed});
	t = 0.7f * (1.0f + sinf(p.z + 10.0f * turbulence(p)));
	return (vec3_lerp(mat->albedo, mat->albedo2, t));
}

/*
** Perlin wood grain pattern.
** Computes concentric cylindrical rings (distance from Y-axis)
** and adds Perlin noise to distort them, simulating natural wood grain
** where rings aren't perfectly circular.
**
** The pattern uses:
**   dist = sqrt(x^2 + z^2) — distance from vertical axis
**   grain = sin(scale * dist + noise * distortion)
** Small noise multiplied by a distortion factor creates subtle
** irregularities in the ring spacing.
**
** @param hit  - Hit record (point is used, bump_offset is written)
** @param mat  - Material with two albedo colors and scale/bump settings
** @return     - Blended color based on wood grain pattern
*/
t_vec3	pattern_perlin_wood(t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	float	grain;
	float	dist;

	scale = mat->pattern_scale;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	p = vec3_scale(p, scale);
	dist = sqrtf(p.x * p.x + p.z * p.z);
	grain = sinf(scale * dist + 4.0f * perlin_noise(p.x, p.y, p.z));
	grain = 0.5f * (grain + 1.0f);
	return (vec3_lerp(mat->albedo, mat->albedo2, grain));
}

/*
** Perlin turbulence pattern.
** Uses raw turbulence (sum of |noise| octaves) directly to blend
** between two colors. Creates a chaotic, organic appearance good
** for rock, clouds, or alien surfaces.
**
** The turbulence value is clamped to [0, 1] since it can occasionally
** exceed 1.0 when multiple octaves align.
**
** @param hit  - Hit record (point is used, bump_offset is written)
** @param mat  - Material with two albedo colors and scale/bump settings
** @return     - Blended color based on turbulence value
*/
t_vec3	pattern_perlin_turb(t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	float	t;

	scale = mat->pattern_scale;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	p = vec3_scale(p, scale);
	t = turbulence(p);
	t = clampf01(t);
	return (vec3_lerp(mat->albedo, mat->albedo2, t));
}
