/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:56 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/30 15:09:39 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "objects.h"
#include "materials.h"
#include "utils.h"
#include "scene.h"

static inline t_vec3	evaluate_light(\
const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel);
static inline t_vec3	sample_light(\
t_vec3 l, float radius_sq, t_vec2 uv, float *pdf);
static inline bool	hit_shadow(\
const t_context *ctx, const t_path *path, t_vec3 hit_biased, float dist);

t_vec3	add_lighting(\
const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	t_vec3			radiance;

	radiance = evaluate_light(ctx, path, light, pixel);
	if (path->bounce > 0)
		radiance = vec3_clamp_mag(radiance, light->max_radiance);
	return (vec3_mul(path->throughput, radiance));
}

static inline t_vec3	evaluate_light(\
const t_context *ctx, t_path *path, const t_light *light, t_pixel *pixel)
{
	const t_vec3	hit_biased = vec3_bias(path->hit.point, path->hit.normal);
	const t_vec3	hit_to_l_center = \
vec3_sub(light->obj->transform.pos, hit_biased);
	t_vec3			radiance;
	float			dist;
	t_vec3			cross;

	random_uv(ctx, path, pixel, BN_CO_U + (light->idx * 2u));
	path->l = \
sample_light(hit_to_l_center, light->radius_sq, path->uv, &path->pdf);
	set_shader_data(path);
	if (path->ndotl <= G_EPSILON)
		return (vec3_n(0.0f));
	cross = vec3_cross(hit_to_l_center, path->l);
	dist = vec3_dot(hit_to_l_center, path->l) - \
sqrtf(fmaxf(0.0f, light->radius_sq - vec3_dot(cross, cross)));
	if (\
hit_shadow(ctx, path, hit_biased, dist - fmaxf(B_EPSILON, dist * G_EPSILON)))
		return (vec3_n(0.0f));
	radiance = vec3_mul(light->emission, bsdf(path));
	radiance = vec3_scale(radiance, (path->ndotl / path->pdf) * \
power_heuristic(path->pdf, bsdf_pdf(path)));
	return (radiance);
}

static inline t_vec3	sample_light(\
t_vec3 l, float radius_sq, t_vec2 uv, float *pdf)
{
	const float		dist_sq = vec3_dot(l, l);
	float			sin_theta_sq;
	float			cos_theta_max;
	t_vec3			res;
	t_vec3			l_dir;

	if (dist_sq <= radius_sq)
		return (vec3_n(0.0f));
	l_dir = vec3_scale(l, 1.0f / sqrtf(dist_sq));
	sin_theta_sq = radius_sq / dist_sq;
	sin_theta_sq = fminf(sin_theta_sq, 0.9999f);
	if (sin_theta_sq < 1e-3f)
	{
		cos_theta_max = 1.0f - (sin_theta_sq * 0.5f);
		*pdf = 1.0f / fmaxf(M_PI * sin_theta_sq, G_EPSILON);
	}
	else
	{
		cos_theta_max = sqrtf(1.0f - sin_theta_sq);
		*pdf = 1.0f / fmaxf((M_TAU * (1.0f - cos_theta_max)), G_EPSILON);
	}
	res = sample_cone(l_dir, cos_theta_max, uv);
	return (res);
}

float	light_pdf(t_vec3 l, float radius_sq)
{
	const float		dist_sq = vec3_dot(l, l);
	float			sin_theta_sq;
	float			cos_theta_max;
	float			solid_angle;
	float			pdf;

	if (dist_sq <= radius_sq)
		return (0.0f);
	sin_theta_sq = radius_sq / dist_sq;
	sin_theta_sq = fminf(sin_theta_sq, 0.9999f);
	if (sin_theta_sq < 1e-3f)
		solid_angle = M_PI * sin_theta_sq;
	else
	{
		cos_theta_max = sqrtf(1.0f - sin_theta_sq);
		solid_angle = M_TAU * (1.0f - cos_theta_max);
	}
	pdf = 1.0f / fmaxf(solid_angle, G_EPSILON);
	return (pdf);
}

static inline bool	hit_shadow(\
const t_context *ctx, const t_path *path, t_vec3 hit_biased, float dist)
{
	const t_ray		shadow_ray = new_ray(hit_biased, path->l);
	t_hit			dummy_hit;

	memset(&dummy_hit, 0, sizeof(t_hit));
	dummy_hit.t = dist;
	if (\
!(path->mat->flags & MAT_NO_REC_SHADOW) && \
(hit_bvh_shadow(\
ctx->scene.geo.bvh_root_idx, &shadow_ray, dist, ctx->scene.geo.bvh_nodes) || \
hit_planes(ctx, &shadow_ray, &dummy_hit)))
		return (true);
	return (false);
}
