/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:49:37 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/27 21:03:12 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline void	nee_editing(const t_context *ctx, t_path *path);
static inline void	write_to_selection_mask(\
const t_context *ctx, t_path *path, t_pixel *pixel);

bool	trace_ray_editing(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		bg_color;
	t_vec3		light_emission;

	if ((int)hit_object(ctx->editor.selected_obj, &path->ray, &path->hit) | \
(int)hit_object(\
ctx->renderer.cam.directional_light.obj, &path->ray, &path->hit) | \
(int)hit_bvh_editing(ctx->scene.geo.bvh_root_idx, &path->ray, &path->hit, \
ctx->scene.geo.bvh_nodes) | (int)hit_planes(ctx, &path->ray, &path->hit))
	{
		write_to_selection_mask(ctx, path, pixel);
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (path->mat->is_emissive)
		{
			light_emission = vec3_mul(path->throughput, path->mat->emission);
			path->color = vec3_add(path->color, light_emission);
			return (false);
		}
		if (ctx->scene.env.has_dir_light)
			add_lighting_editing(ctx, path, \
&ctx->renderer.cam.directional_light);
		if (ctx->scene.env.lights.total > 0)
			nee_editing(ctx, path);
		ambient_lighting(path, &ctx->scene.env.amb_light);
		return (false);
	}
	ctx->editor.selection_mask[pixel->y * ctx->renderer.width + pixel->x] = \
-M_INF;
	bg_color = \
background_color(&ctx->scene, &path->ray, ctx->renderer.cam.skydome_uv_offset);
	bg_color = vec3_scale(bg_color, ctx->scene.env.amb_light.intensity);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee_editing(const t_context *ctx, t_path *path)
{
	const t_light	*light;
	size_t			i;

	i = 0;
	while (i < ctx->scene.env.lights.total)
	{
		light = ((t_light **)ctx->scene.env.lights.items)[i++];
		add_lighting_editing(ctx, path, light);
	}
}

static inline void	write_to_selection_mask(\
const t_context *ctx, t_path *path, t_pixel *pixel)
{
	float		*mask;

	mask = \
&ctx->editor.selection_mask[pixel->y * ctx->renderer.width + pixel->x];
	if (path->hit.obj == ctx->editor.selected_obj)
		*mask = path->hit.t;
	else
		*mask = -path->hit.t;
}
