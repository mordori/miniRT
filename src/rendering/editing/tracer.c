#include "rendering.h"
#include "scene.h"
#include "lights.h"
#include "objects.h"
#include "utils.h"
#include "materials.h"

static inline void	nee_editing(const t_context *ctx, t_path *path, t_pixel *pixel);
// static inline bool	scatter_editing(const t_context *ctx, t_path *path, t_pixel *pixel);

bool	trace_ray_editing(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	t_vec3		bg_color;

	if ((int)hit_object(ctx->scene.selected_obj, &path->ray, &path->hit) | \
(int)hit_bvh(ctx->scene.bvh_root, &path->ray, &path->hit, 0))
	{
		path->mat = path->hit.obj->mat;
		set_material_data(path);
		if (ctx->scene.has_directional_light)
			add_lighting_editing(ctx, path, &ctx->renderer.cam.directional_light);
		if (ctx->scene.lights.total > 0)
			nee_editing(ctx, path, pixel);
		ambient_lighting(path, &ctx->scene.ambient_light);
		// return (scatter_editing(ctx, path, pixel));
		return (false);
	}
	bg_color = background_color(&ctx->scene.skydome, &path->ray, ctx->renderer.cam.skydome_uv_offset);
	path->color = vec3_add(path->color, vec3_mul(path->throughput, bg_color));
	return (false);
}

static inline void	nee_editing(const t_context *ctx, t_path *path, t_pixel *pixel)
{
	const t_light	*light;
	size_t			i;

	if (ctx->renderer.mode == RENDER_PREVIEW)
	{
		i = (uint32_t)(blue_noise(&ctx->tex_bn, pixel, BN_LI) * ctx->scene.lights.total);
		if (i >= ctx->scene.lights.total)
			i = ctx->scene.lights.total - 1;
		light = ((t_light **)ctx->scene.lights.items)[i];
		add_lighting_editing(ctx, path, light);
		path->color = vec3_scale(path->color, (float)ctx->scene.lights.total);
	}
	else
	{
		i = 0;
		while (i < ctx->scene.lights.total)
		{
			light = ((t_light **)ctx->scene.lights.items)[i++];
			add_lighting_editing(ctx, path, light);
		}
	}
}

// static inline bool	scatter_editing(const t_context *ctx, t_path *path, t_pixel *pixel)
// {
// 	t_vec3		f;
// 	float		p;

// 	random_uv(ctx, path, pixel, BN_SC_U);
// 	f = vec3_schlick(path->f0, path->ndotv);
// 	p = fmaxf(fmaxf(f.r, f.g), f.b);
// 	path->p_spec = clampf(p, 0.1f, 0.9f);
// 	if (path->mat->metallic >= 0.9f)
// 		path->p_spec = 1.0f;
// 	path->sample_spec = randomf01(pixel->seed) < path->p_spec;
// 	if (path->sample_spec)
// 	{
// 		path->v = vec3_negate(path->ray.dir);
// 		path->h = vec3_normalize(vec3_add(path->v, path->l));
// 		path->l = vec3_reflect(path->ray.dir, path->h);
// 	}
// 	else
// 		path->l = sample_cos_hemisphere(path->n, path->uv);
// 	path->ndotl = clampf01(vec3_dot(path->n, path->l));
// 	if (path->ndotl <= 1e-12f)
// 		return (false);
// 	path->throughput = vec3_mul(path->throughput, weight);
// 	path->ray = new_ray(vec3_bias(path->hit.point, path->n), path->l);
// 	++path->bounce;
// 	return (true);
// }
