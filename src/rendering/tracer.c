/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:50:16 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:50:18 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"
#include "camera.h"

static inline bool	trace_ray(\
const t_context *ctx, t_path *path, t_pixel *pixel, t_render_mode mode);
static inline void	nee(\
const t_context *ctx, t_path *path, t_pixel *pixel, t_render_mode mode);
static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel);
static inline bool	eval_emissive(\
const t_context *ctx, t_path *path, t_render_mode mode);

t_vec3	trace_path(\
const t_context *ctx, t_pixel *pixel, t_render_mode mode, uint8_t bounces)
{
	const t_renderer	*r = &ctx->renderer;
	t_path				path;
	t_vec3				pixel_loc;
	t_vec3				pixel_dir;
	t_vec3				ray_orig;

	memset(&path, 0, sizeof(t_path));
	pixel_loc = vec3_add(vec3_scale(r->cam.viewport.d_u, pixel->u), \
vec3_scale(r->cam.viewport.d_v, pixel->v));
	pixel_loc = vec3_add(r->cam.viewport.pixel_00_loc, pixel_loc);
	pixel_dir = vec3_normalize(vec3_sub(pixel_loc, r->cam.transform.pos));
	ray_orig = sample_defocus_disk(ctx, pixel);
	path.ray = new_ray(ray_orig, vec3_normalize(vec3_sub(vec3_add(\
r->cam.transform.pos, vec3_scale(pixel_dir, \
(r->cam.focus_dist / vec3_dot(pixel_dir, r->cam.forward)))), ray_orig)));
	path.throughput = (t_vec3){{1.0f, 1.0f, 1.0f}};
	while (path.bounce < bounces)
	{
		path.hit = new_hit(path.bounce);
		if (mode == SOLID && !trace_ray_editing(ctx, &path, pixel))
			break ;
		else if (mode != SOLID && !trace_ray(ctx, &path, pixel, mode))
			break ;
	}
	if (vec3_is_nan_inf(path.color))
		return (vec3_n(0.0f));
	return (path.color);
}

static inline bool	eval_emissive(\
const t_context *ctx, t_path *path, t_render_mode mode)
{
	float		weight;
	float		pdf;
	t_vec3		light_emission;

	if (path->mat->is_emissive)
	{
		if (path->bounce == 0)
			light_emission = path->mat->emission;
		else
		{
			pdf = light_pdf(vec3_sub(\
path->ray.origin, path->hit.obj->transform.pos), \
path->hit.obj->shape.sphere.radius_sq);
			if (mode == PREVIEW && ctx->scene.env.lights.total > 0)
				pdf /= (float)ctx->scene.env.lights.total;
			weight = power_heuristic(path->pdf, pdf);
			light_emission = vec3_mul(path->throughput, \
vec3_scale(path->mat->emission, weight));
			light_emission = vec3_clamp_mag(light_emission, MAX_RADIANCE);
		}
		path->color = vec3_add(path->color, light_emission);
		return (true);
	}
	return (false);
}

static inline bool	trace_ray(\
const t_context *ctx, t_path *path, t_pixel *pixel, t_render_mode mode)
{
	t_vec3		bg_color;

	if (\
(int)hit_object(\
ctx->renderer.cam.directional_light.obj, &path->ray, &path->hit) | \
(int)hit_bvh(ctx->scene.geo.bvh_root_idx, &path->ray, &path->hit, \
ctx->scene.geo.bvh_nodes) | \
(int)hit_planes(ctx, &path->ray, &path->hit))
	{
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (eval_emissive(ctx, path, mode))
			return (false);
		if (ctx->scene.env.has_dir_light)
			path->color = vec3_add(path->color, \
add_lighting(ctx, path, &ctx->renderer.cam.directional_light, pixel));
		if (ctx->scene.env.lights.total > 0)
			nee(ctx, path, pixel, mode);
		return (scatter(ctx, path, pixel));
	}
	bg_color = background_color(\
&ctx->scene, &path->ray, ctx->renderer.cam.skydome_uv_offset);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee(\
const t_context *ctx, t_path *path, t_pixel *pixel, t_render_mode mode)
{
	const t_light		*light;
	const uint32_t		li_dim = BN_LI + (path->bounce * ctx->bn_stride);
	size_t				i;
	float				li_rand;

	if (mode == PREVIEW)
	{
		if (path->bounce == 0)
			li_rand = blue_noise(&ctx->tex_bn, pixel, li_dim);
		else
			li_rand = r1_sequence(pixel->frame + (path->bounce * FP_PRIME), \
static_blue_noise(&ctx->tex_bn, pixel, li_dim));
		i = (uint32_t)(li_rand * ctx->scene.env.lights.total);
		if (i >= ctx->scene.env.lights.total)
			i = ctx->scene.env.lights.total - 1;
		light = ((t_light **)ctx->scene.env.lights.items)[i];
		path->color = vec3_add(path->color, vec3_scale(\
add_lighting(ctx, path, light, pixel), (float)ctx->scene.env.lights.total));
		return ;
	}
	i = 0;
	while (i < ctx->scene.env.lights.total)
	{
		light = ((t_light **)ctx->scene.env.lights.items)[i++];
		path->color = \
vec3_add(path->color, add_lighting(ctx, path, light, pixel));
	}
}

static inline bool	scatter(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	const t_vec3	f = vec3_schlick(path->f0, path->ndotv);
	const float		p = fmaxf(fmaxf(f.r, f.g), f.b);
	const uint32_t	spec_dim = BN_SR_U + (path->bounce * ctx->bn_stride);
	float			spec_rand;

	path->p_spec = clampf(p, 0.1f, 0.9f);
	if (path->mat->metallic >= 0.9f)
		path->p_spec = 1.0f;
	if (path->bounce == 0)
		spec_rand = blue_noise(&ctx->tex_bn, pixel, spec_dim);
	else
		spec_rand = r1_sequence(pixel->frame + (path->bounce * FP_PRIME), \
static_blue_noise(&ctx->tex_bn, pixel, spec_dim));
	path->sample_spec = spec_rand <= path->p_spec && spec_rand >= 0.0f;
	random_uv(ctx, path, pixel, BN_SC_U);
	if (!sample_bsdf(path))
		return (false);
	path->ray = new_ray(vec3_bias(path->hit.point, path->hit.normal), path->l);
	return (russian_roulette(path, pixel));
}
