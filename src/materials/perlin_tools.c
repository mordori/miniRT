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

/*
** Per-object noise offset. Hashes the world position with the canonical
** GLSL sin/fract trick: sinf maps the dot product to [-1, 1], then the
** large multiplier (43758.5453, tuned for that range) scrambles the
** mantissa bits into a uniform [0, 2pi) scalar.
** Golden-ratio multiples {s, s*phi, s*(phi+1)} give three independent
** per-axis offsets so each object samples a distinct noise region.
*/
static inline t_vec3	object_seed(const t_object *obj)
{
	float	s;

	s = sinf(obj->transform.pos.x * 12.9898f
			+ obj->transform.pos.y * 78.233f
			+ obj->transform.pos.z * 37.719f) * 43758.5453f;
	s = (s - floorf(s)) * M_TAU;
	return ((t_vec3){s, s * 1.6180339f, s * 2.6180339f});
}

/*
** 1/f fractal noise: 7 octaves, each doubling frequency and halving
** amplitude. fabsf() folds smooth zero-crossings into sharp cusps,
** creating the jagged ridges used for marble veins and rock.
** Output is always positive, typically [0, ~2] when octaves align.
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
** Marble veins: sin(p.z + 10 * turbulence(p)) blended over two colors.
** p.z alone gives flat parallel bands; the x10 turbulence distortion
** kinks and folds them into irregular veins via its sharp fabsf cusps.
** object_seed() shifts all three noise axes with golden-ratio offsets
** so each object samples a distinct region of the noise field.
*/
t_vec3	pattern_perlin_marble(t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	float	t;

	scale = mat->pattern_scale;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	p = vec3_scale(p, scale);
	p = vec3_add(p, object_seed(hit->obj));
	t = 0.7f * (1.0f + sinf(p.z + 10.0f * turbulence(p)));
	return (vec3_lerp(mat->albedo, mat->albedo2, t));
}

/*
** Wood grain: concentric cylindrical rings (dist from Y-axis) gently
** distorted by smooth perlin_noise. Raw perlin_noise is used instead
** of turbulence because wood rings undulate smoothly — turbulence's
** fabsf cusps would make them look cracked rather than organic.
** object_seed() offsets the ring centre, as if cut from a different
** part of the trunk.
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
	p = vec3_add(p, object_seed(hit->obj));
	dist = sqrtf(p.x * p.x + p.z * p.z);
	grain = sinf(scale * dist + 4.0f * perlin_noise(p.x, p.y, p.z));
	grain = 0.5f * (grain + 1.0f);
	return (vec3_lerp(mat->albedo, mat->albedo2, grain));
}

/*
** Raw turbulence blend: maps turbulence output directly to two colors.
** No sin() wrapper — the fractal noise drives the blend, giving chaotic
** organic shapes suited for rock, clouds, or alien surfaces.
** Clamped to [0, 1] since multiple aligned octaves can exceed 1.0.
*/
t_vec3	pattern_perlin_turb(t_hit *hit, const t_material *mat)
{
	float	scale;
	t_vec3	p;
	float	t;

	scale = mat->pattern_scale;
	p = vec3_sub(hit->point, hit->obj->transform.pos);
	p = vec3_scale(p, scale);
	p = vec3_add(p, object_seed(hit->obj));
	t = turbulence(p);
	t = clampf01(t);
	return (vec3_lerp(mat->albedo, mat->albedo2, t));
}
