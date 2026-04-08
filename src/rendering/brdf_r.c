/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brdf_r.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:42 by myli-pen          #+#    #+#             */
/*   Updated: 2026/04/08 15:51:58 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

static inline float	d_ggx(float ndoth, float a);
static inline float	v_smith_ggx(float ndotv, float ndotl, float alpha);

float	reflectance(float ior)
{
	float		reflectance;

	reflectance = (ior - 1.0f) / (ior + 1.0f);
	reflectance *= reflectance;
	return (reflectance);
}

float	pdf_ggx_vndf(const t_path *path)
{
	const float		a2 = path->alpha * path->alpha;
	float			d;
	float			pdf;
	float			den;

	d = d_ggx(path->ndoth, path->alpha);
	den = path->ndotv + sqrtf(a2 + (1.0f - a2) * (path->ndotv * path->ndotv));
	pdf = d / fmaxf(2.0f * den, G_EPSILON);
	return (pdf);
}

t_vec3	cook_torrance_brdf(const t_path *path)
{
	float		d;
	float		v;
	t_vec3		f;

	d = d_ggx(path->ndoth, path->alpha);
	v = v_smith_ggx(path->ndotv, path->ndotl, path->alpha);
	f = vec3_schlick(path->f0, path->vdoth);
	return (vec3_scale(f, d * v));
}

// Trowbridge-Reitz
static inline float	d_ggx(float ndoth, float a)
{
	const float		a2 = a * a;
	float			den;

	den = (ndoth * ndoth) * (a2 - 1.0f) + 1.0f;
	den = M_PI * (den * den);
	return (a2 / den);
}

static inline float	v_smith_ggx(float ndotv, float ndotl, float a)
{
	const float		a2 = a * a;
	float			ggx_v;
	float			ggx_l;

	ggx_v = ndotl * sqrtf((ndotv * ndotv) * (1.0f - a2) + a2);
	ggx_l = ndotv * sqrtf((ndotl * ndotl) * (1.0f - a2) + a2);
	return (0.5f / fmaxf((ggx_v + ggx_l), G_EPSILON));
}
