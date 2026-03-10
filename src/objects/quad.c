/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:53:47 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:54:27 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"
#include "utils.h"

/*
** Initialize a quad (parallelogram) and add it to the scene.
**
** A quad is defined by a corner point Q and two edge vectors u and v.
** The surface spans: P(α,β) = Q + α*u + β*v, where α,β ∈ [0,1].
**
** Precomputed: normal, w (parametric coord helper), d (plane const), area.
*/
t_error	init_quad(t_context *ctx, t_quad *quad, uint32_t mat_id)
{
	t_object	obj;
	t_vec3		n_cross;
	t_vec3		w_temp;
	float		n_len_sq;

	n_cross = vec3_cross(quad->u, quad->v);
	n_len_sq = vec3_dot(n_cross, n_cross);
	if (n_len_sq < G_EPSILON)
		return (E_RANGE);
	quad->area = sqrtf(n_len_sq);
	quad->normal = vec3_scale(n_cross, 1.0f / quad->area);
	quad->w = vec3_scale(n_cross, 1.0f / n_len_sq);
	quad->d = vec3_dot(quad->normal, quad->q);
	w_temp = vec3_scale(n_cross, 1.0f / n_len_sq);
	quad->vec_alpha = vec3_cross(quad->v, w_temp);
	quad->vec_beta = vec3_cross(w_temp, quad->u);
	obj = (t_object){0};
	obj.type = OBJ_QUAD;
	obj.shape.quad = *quad;
	obj.material_id = mat_id;
	obj.transform.pos = vec3_add(quad->q, vec3_scale(
				vec3_add(quad->u, quad->v), 0.5f));
	return (add_object(ctx, &obj));
}

/*
** Check if ray-plane hit point lies within the quad parallelogram.
** Uses Cramer's rule: α = dot(w, cross(p, v)), β = dot(w, cross(u, p)).
** α,β ∈ [0,1] means inside. They also become the UV coordinates.
*/
static bool	quad_hit_record(const t_quad *quad, const t_ray *ray,
				t_hit *hit, float t)
{
	t_vec3		hit_point;
	t_vec3		p;
	float		alpha;
	float		beta;

	hit_point = vec3_add(ray->origin, vec3_scale(ray->dir, t));
	p = vec3_sub(hit_point, quad->q);
	alpha = vec3_dot(quad->vec_alpha, p);
	if (alpha < 0.0f || alpha > 1.0f)
		return (false);
	beta = vec3_dot(quad->vec_beta, p);
	if (beta < 0.0f || beta > 1.0f)
		return (false);
	hit->t = t;
	hit->point = hit_point;
	hit->normal = quad->normal;
	if (vec3_dot(quad->normal, ray->dir) > 0)
		hit->normal = vec3_scale(hit->normal, -1.0f);
	hit->uv = vec2(alpha, beta);
	return (true);
}

/*
** Ray-quad intersection: intersect the plane, then bounds-check.
*/
bool	hit_quad(const t_shape *shape, const t_ray *ray, t_hit *hit)
{
	const t_quad	*quad;
	float			denom;
	float			t;

	quad = &shape->quad;
	denom = vec3_dot(quad->normal, ray->dir);
	if (fabsf(denom) < 1e-6f)
		return (false);
	t = (quad->d - vec3_dot(quad->normal, ray->origin)) / denom;
	if (t < 1e-3f || t >= hit->t)
		return (false);
	return (quad_hit_record(quad, ray, hit, t));
}
