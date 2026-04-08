/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:33 by myli-pen          #+#    #+#             */
/*   Updated: 2026/04/08 15:51:02 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "objects.h"
#include "materials.h"
#include "utils.h"
#include "scene.h"

static inline t_vec3	direct_lighting_editing(\
const t_context *ctx, t_path *path, const t_light *light);
static inline t_vec3	add_light(\
const t_context *ctx, const t_path *path, const t_light *light, float dist);
static inline bool	hit_shadow(\
const t_context *ctx, const t_path *path, t_vec3 hit_biased, float dist);

void	add_lighting_editing(\
const t_context *ctx, t_path *path, const t_light *light)
{
	t_vec3		radiance;

	radiance = direct_lighting_editing(ctx, path, light);
	radiance = vec3_clamp_mag(radiance, light->max_radiance);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, radiance));
}

static inline t_vec3	direct_lighting_editing(\
const t_context *ctx, t_path *path, const t_light *light)
{
	t_vec3		l;
	t_vec3		hit_biased;
	float		dist_sq;
	float		dist;

	path->n = path->hit.normal;
	hit_biased = vec3_bias(path->hit.point, path->n);
	l = vec3_sub(light->obj->transform.pos, hit_biased);
	dist_sq = vec3_dot(l, l);
	if (dist_sq < LEN_SQ_EPSILON)
		return (vec3_n(0.0f));
	dist = sqrtf(dist_sq);
	path->l = vec3_scale(l, 1.0f / dist);
	path->ndotl = clampf01(vec3_dot(path->n, path->l));
	if (path->ndotl <= G_EPSILON)
		return (vec3_n(0.0f));
	if (hit_shadow(ctx, path, hit_biased, \
fmaxf(B_EPSILON, (dist - light->radius) * G_EPSILON)))
		return (vec3_n(0.0f));
	return (add_light(ctx, path, light, dist_sq));
}

static inline bool	hit_shadow(\
const t_context *ctx, const t_path *path, t_vec3 hit_biased, float dist)
{
	t_ray		shadow_ray;
	t_hit		dummy_hit;

	shadow_ray = new_ray(hit_biased, path->l);
	memset(&dummy_hit, 0, sizeof(t_hit));
	dummy_hit.t = dist;
	if (\
!(path->mat->flags & MAT_NO_REC_SHADOW) && \
((hit_object(ctx->editor.selected_obj, &shadow_ray, &dummy_hit) && \
!(ctx->editor.selected_obj->flags & OBJ_NO_CAST_SHADOW)) || \
hit_bvh_shadow(\
ctx->scene.geo.bvh_root_idx, &shadow_ray, dist, ctx->scene.geo.bvh_nodes) || \
hit_planes(ctx, &shadow_ray, &dummy_hit)))
		return (true);
	return (false);
}

void	ambient_lighting(t_path *path, const t_light *light)
{
	t_vec3		res;

	res = vec3_mul(light->color, get_surface_color(path->mat, &path->hit));
	res = vec3_scale(res, light->intensity);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, res));
}

static inline t_vec3	add_light(\
const t_context *ctx, const t_path *path, const t_light *light, float dist_sq)
{
	t_vec3		res;
	float		attenuation;
	float		factor_radius;

	factor_radius = 1.0f;
	if (light != &ctx->renderer.cam.directional_light)
		factor_radius = fmaxf(light->radius_sq, 1.0f) * light->radius;
	attenuation = \
light->intensity * factor_radius * path->ndotl / fmax(dist_sq, G_EPSILON);
	res = vec3_mul(get_surface_color(path->mat, &path->hit), light->color);
	res = vec3_scale(res, attenuation * M_1_PI);
	return (res);
}
