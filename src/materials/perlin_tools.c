#include "materials.h"
#include <math.h>

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
** Compute procedural bump offset from noise via finite differences.
** Evaluates the noise gradient by sampling at slightly displaced
** positions along each axis, producing a surface perturbation vector.
**
** The bump_strength parameter controls the intensity of the effect.
** An epsilon of 0.01 provides good visual quality without aliasing.
**
** @param p         - Point in scaled world space
** @param strength  - Bump intensity multiplier
** @return          - 3D bump offset vector (perturbation direction)
*/
t_vec3	compute_bump_offset(t_vec3 p, float strength)
{
	float	eps;
	float	center;
	t_vec3	offset;

	eps = 0.01f;
	center = turbulence(p);
	offset.x = (turbulence(vec3(p.x + eps, p.y, p.z)) - center) / eps;
	offset.y = (turbulence(vec3(p.x, p.y + eps, p.z)) - center) / eps;
	offset.z = (turbulence(vec3(p.x, p.y, p.z + eps)) - center) / eps;
	return (vec3_scale(offset, strength));
}
